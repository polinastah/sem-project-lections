#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    width(900),
    height(500),
    centralWidget(new QWidget(this)),
    mainLayout(new QVBoxLayout(centralWidget)),
    userTable(new QTableView(centralWidget)),
    userTableModel(new QStandardItemModel(this)),
    booksTable(new QTableView(centralWidget)),
    booksTableModel(new QStandardItemModel(this)),
    menuButton(new MenuButton("Menu", centralWidget)),
    library()
{
    connect(menuButton, &MenuButton::addBookRequested,
            this, &MainWindow::handleAddBookRequested);
    connect(menuButton, &MenuButton::deleteBookRequested,
            this, &MainWindow::handleDeleteBookRequested);
    connect(menuButton, &MenuButton::addUserRequested,
            this, &MainWindow::handleAddUserRequested);
    connect(menuButton, &MenuButton::deleteUserRequested,
            this, &MainWindow::handleDeleteUserRequested);
    connect(menuButton, &MenuButton::findUserRequested,
            this, &MainWindow::handleFindUserRequested);
    connect(menuButton, &MenuButton::findBookRequested,
            this, &MainWindow::handleFindBookRequested);
    connect(userTable, &QTableView::clicked, this, &MainWindow::handleUserTableClicked);

    setCentralWidget(centralWidget);
    setWindowTitle("Library");
    setGeometry(QApplication::screens().at(0)->size().width() / 2 - width / 2,
                QApplication::screens().at(0)->size().height() / 2 - height / 2,
                width, height);

    library.addToLibrary("Martin Iden", "J.London", "1963");
    library.addToLibrary("The Great Gatsby", "F.S.Fitzherald", "1950");
    library.addUser("Polina", "1204");
    library.addUser("Diana", "2606");
    library.borrowBook("1204", "Martin Iden", "J.London");

    setupLayout();

    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    menuButton->setFixedSize(100, 30);
}

void MainWindow::setupUserTable() {
    userTableModel->setHorizontalHeaderLabels({"ID", "Name"});
    userTable->setModel(userTableModel);
    updateUsersTable();

    userTable->setShowGrid(true);
    userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::setupBooksTable()
{
    booksTableModel->setHorizontalHeaderLabels({"Title", "Author", "Year"});
    booksTable->setModel(booksTableModel);
    updateBooksTable();

    booksTable->setShowGrid(true);
    booksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::updateBooksTable()
{
    booksTableModel->clear();
    booksTableModel->setHorizontalHeaderLabels({"Title", "Author", "Year"});

    const auto& books = library.getBooks();
    for (const auto& book : books) {
        booksTableModel->appendRow({
            new QStandardItem(book->getTitle()),
            new QStandardItem(book->getAuthor()),
            new QStandardItem(book->getYear())
        });
    }
}

void MainWindow::setupLayout()
{
    setupUserTable();
    setupBooksTable();

    QHBoxLayout *tablesLayout = new QHBoxLayout();
    tablesLayout->addWidget(userTable);
    tablesLayout->addWidget(booksTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(menuButton);

    mainLayout->addLayout(tablesLayout);
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::handleAddBookRequested(const QString& title,
                                        const QString& author, const QString& year) {
    library.addToLibrary(title, author, year);
    updateBooksTable();
}

void MainWindow::handleDeleteBookRequested(const QString& title,
                                           const QString& author) {
    library.deleteFromLibrary(title, author);
    updateBooksTable();
}

void MainWindow::updateUsersTable() {
    userTableModel->clear();
    userTableModel->setHorizontalHeaderLabels({"ID", "Name"});

    const auto& users = library.getUsers();
    for (const auto& user : users) {
        userTableModel->appendRow({
            new QStandardItem(user.first->getID()),
            new QStandardItem(user.first->getName())
        });
    }
}

void MainWindow::handleAddUserRequested(const QString& name, const QString& id) {
    library.addUser(name, id);
    updateUsersTable();
}

void MainWindow::handleDeleteUserRequested(const QString& name, const QString& id) {
    library.deleteUser(name, id);
    updateUsersTable();
}

void MainWindow::handleFindUserRequested(const QString& name, const QString& id) {
    QWidgetList widgets = QApplication::topLevelWidgets();
    for (QWidget* widget : widgets) {
        if (widget->isWindow() && widget != this) {
            widget->close();
        }
    }

    int row = 0;

    for (const auto& i : library.getUsers()) {
        bool nameMatch = name.isEmpty() || i.first->getName().contains(name, Qt::CaseInsensitive);
        bool idMatch = id.isEmpty() || i.first->getID().contains(id, Qt::CaseInsensitive);

        if (nameMatch && idMatch) {
            userTable->selectRow(row);
            userTable->scrollTo(userTableModel->index(row, 0));
            return;
        }
        row++;
    }

    QMessageBox::information(this, "Error", "User wasn't found");
}

void MainWindow::handleFindBookRequested(const QString& title, const QString& author) {
    QWidgetList widgets = QApplication::topLevelWidgets();
    for (QWidget* widget : widgets) {
        if (widget->isWindow() && widget != this) {
            widget->close();
        }
    }

    for (int i = 0; i < library.getBooks().size(); ++i) {
        bool titleMatch = title.isEmpty() ||
                          library.getBooks()[i]->getTitle().contains(title, Qt::CaseInsensitive);
        bool authorMatch = author.isEmpty() ||
                           library.getBooks()[i]->getAuthor().contains(author, Qt::CaseInsensitive);

        if (titleMatch && authorMatch) {
            booksTable->selectRow(i);
            booksTable->scrollTo(booksTableModel->index(i, 0));
            return;
        }
    }
    QMessageBox::information(this, "Error", "Book wasn't found");
}

void MainWindow::handleUserTableClicked(const QModelIndex &index) {
    if (index.row() < 0 || index.row() >= userTableModel->rowCount()) return;

    QString userId = userTableModel->index(index.row(), 0).data().toString();
    std::shared_ptr<User> selectedUser = nullptr;

    for (const auto& [user, books] : library.getUsers()) {
        if (user->getID() == userId) {
            selectedUser = user;
            break;
        }
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("%1's Books").arg(selectedUser->getName()));
    dialog.setMinimumSize(600, 400);

    QTableView *booksView = new QTableView(&dialog);
    QStandardItemModel *booksModel = new QStandardItemModel(&dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *borrowButton = new QPushButton("Borrow book", &dialog);
    QPushButton *returnButton = new QPushButton("Return book", &dialog);

    auto refreshBooks = [&]() {
        booksModel->clear();
        booksModel->setHorizontalHeaderLabels({"Title", "Author", "Year"});
        const auto& userBooks = library.getUsers().at(selectedUser);
        for (const auto& book : userBooks) {
            booksModel->appendRow({
                new QStandardItem(book->getTitle()),
                new QStandardItem(book->getAuthor()),
                new QStandardItem(book->getYear())
            });
        }
    };

    refreshBooks();

    booksView->setModel(booksModel);
    booksView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    booksView->setSelectionBehavior(QAbstractItemView::SelectRows);
    booksView->setSelectionMode(QAbstractItemView::SingleSelection);
    booksView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(returnButton, &QPushButton::clicked, [&]() {
        QModelIndexList selected = booksView->selectionModel()->selectedRows();
        if (selected.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "Please select a book to return");
            return;
        }

        int selectedRow = selected.first().row();
        QString title = booksModel->index(selectedRow, 0).data().toString();
        QString author = booksModel->index(selectedRow, 1).data().toString();

        if (library.returnBook(userId, title, author)) {
            refreshBooks();
            updateBooksTable();
            QMessageBox::information(&dialog, "Success", "Book returned successfully");
        } else {
            QMessageBox::warning(&dialog, "Error", "Failed to return book");
        }
    });

    connect(borrowButton, &QPushButton::clicked, [&]() {
        showBorrowBookDialog(userId);
        refreshBooks();
    });

    buttonLayout->addStretch();
    buttonLayout->addWidget(borrowButton);
    buttonLayout->addWidget(returnButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addWidget(booksView);
    mainLayout->addLayout(buttonLayout);
    dialog.setLayout(mainLayout);

    dialog.exec();
}

void MainWindow::showBorrowBookDialog(const QString& userId) {
    QDialog dialog(this);
    dialog.setWindowTitle("Borrow Book");

    QFormLayout form(&dialog);

    QLineEdit titleInput(&dialog);
    QLineEdit authorInput(&dialog);

    form.addRow("Title:", &titleInput);
    form.addRow("Author:", &authorInput);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, [&]() {
        QString title = titleInput.text().trimmed();
        QString author = authorInput.text().trimmed();

        if (title.isEmpty() || author.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "Please enter both title and author");
            return;
        }

        if (library.borrowBook(userId, title, author)) {
            updateBooksTable();
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Error", "Failed to borrow book. It may not be available.");
        }
    });

    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    dialog.exec();
}
