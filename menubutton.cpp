#include "menubutton.h"
#include <QVBoxLayout>
#include <QMessageBox>

MenuButton::MenuButton(const QString& text, QWidget* parent)
    : QPushButton("Menu", parent), menuDialog(nullptr)
{
    setFixedSize(100, 30);
    connect(this, &QPushButton::clicked, this, &MenuButton::showMenuDialog);
}

QDialog* MenuButton::createFormDialog(const QString& title, const QStringList& fields)
{
    QDialog* dialog = new QDialog(parentWidget());
    dialog->setWindowTitle(title);

    QFormLayout* form = new QFormLayout(dialog);
    QVector<QLineEdit*> lineEdits;

    for (const auto& field : fields) {
        QLineEdit* lineEdit = new QLineEdit(dialog);
        form->addRow(field, lineEdit);
        lineEdits.append(lineEdit);
    }

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, [=]() {
        QStringList values;
        for (auto edit : lineEdits) {
            values << edit->text();
        }
        dialog->accept();

        if (title == "Add User") {
            emit addUserRequested(values[0], values[1]);
        }
        else if (title == "Add Book") {
            emit addBookRequested(values[0], values[1], values[2]);
        }
        else if (title == "Delete User") {
            emit deleteUserRequested(values[0], values[1]);
        }
        else if (title == "Delete Book") {
            emit deleteBookRequested(values[0], values[1]);
        }
        else if (title == "Find User") {
            emit findUserRequested(values[0], values[1]);
        }
        else if (title == "Find Book") {
            emit findBookRequested(values[0], values[1]);
        }
    });

    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    return dialog;
}

void MenuButton::showMenuDialog()
{
    if (!menuDialog) {
        menuDialog = new QDialog(parentWidget());
        menuDialog->setWindowTitle("Library Menu");
        menuDialog->setFixedSize(300, 300);

        QVBoxLayout* layout = new QVBoxLayout(menuDialog);

        struct ButtonAction {
            QString text;
            const char* slot;
        };

        const ButtonAction actions[] = {
            {"Add User", SLOT(showAddUserDialog())},
            {"Add Book", SLOT(showAddBookDialog())},
            {"Delete User", SLOT(showDeleteUserDialog())},
            {"Delete Book", SLOT(showDeleteBookDialog())},
            {"Find User", SLOT(showFindUserDialog())},
            {"Find Book", SLOT(showFindBookDialog())}
        };

        for (const auto& action : actions) {
            QPushButton* btn = new QPushButton(action.text, menuDialog);
            btn->setFixedHeight(40);
            layout->addWidget(btn);
            connect(btn, SIGNAL(clicked()), this, action.slot);
        }
    }
    menuDialog->exec();
}

void MenuButton::showAddUserDialog() {
    QDialog dialog(parentWidget());
    dialog.setWindowTitle("Add User");

    QFormLayout form(&dialog);

    QLineEdit nameInput(&dialog);
    QLineEdit idInput(&dialog);

    form.addRow("Name:", &nameInput);
    form.addRow("ID (numbers only):", &idInput);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, [&]() {
        QString name = nameInput.text().trimmed();
        QString id = idInput.text().trimmed();

        bool ok;
        id.toInt(&ok);

        if (name.isEmpty() || id.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "All fields must be filled");
            return;
        }

        if (!ok) {
            QMessageBox::warning(&dialog, "Error", "ID must contain only numbers");
            return;
        }

        emit addUserRequested(name, id);
        dialog.accept();
    });

    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    dialog.exec();
}


void MenuButton::showAddBookDialog() {
    QDialog dialog(parentWidget());
    dialog.setWindowTitle("Add Book");

    QFormLayout form(&dialog);

    QLineEdit titleInput(&dialog);
    QLineEdit authorInput(&dialog);
    QLineEdit yearInput(&dialog);

    form.addRow("Title:", &titleInput);
    form.addRow("Author:", &authorInput);
    form.addRow("Year (numbers only):", &yearInput);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, [&]() {
        QString title = titleInput.text().trimmed();
        QString author = authorInput.text().trimmed();
        QString year = yearInput.text().trimmed();

        bool ok;
        year.toInt(&ok);

        if (title.isEmpty() || author.isEmpty() || year.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "All fields must be filled");
            return;
        }

        if (!ok) {
            QMessageBox::warning(&dialog, "Error", "Year must contain only numbers");
            return;
        }

        emit addBookRequested(title, author, year);
        dialog.accept();
    });

    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    dialog.exec();
}


void MenuButton::showDeleteUserDialog()
{
    QDialog* dialog = createFormDialog("Delete User", {"Name:", "ID:"});
    dialog->exec();
    dialog->deleteLater();
}

void MenuButton::showDeleteBookDialog()
{
    QDialog* dialog = createFormDialog("Delete Book", {"Title:", "Author:"});
    dialog->exec();
    dialog->deleteLater();
}

void MenuButton::showFindUserDialog() {
    QDialog* dialog = createFormDialog("Find User", {"Name:", "ID:"});
    dialog->exec();
    dialog->deleteLater();
}

void MenuButton::showFindBookDialog() {
    QDialog* dialog = createFormDialog("Find Book", {"Title:", "Author:"});
    dialog->exec();
    dialog->deleteLater();
}
