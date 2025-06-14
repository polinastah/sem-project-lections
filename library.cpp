#include "library.h"

Library::Library() {}

void Library::addToLibrary(std::shared_ptr<Book> book) {
    library.push_back(book);
}

void Library::addToLibrary(QString title, QString author, QString year) {
    library.push_back(std::make_shared<Book>(title, author, year));
}

void Library::deleteFromLibrary(std::shared_ptr<Book> book) {
    for (int i = 0; i < library.size(); ++i) {
        if (book == library[i]) {
            library.erase(library.begin() + i);
        }
    }
}

void Library::deleteFromLibrary(QString title, QString author) {
    library.erase(
        std::remove_if(library.begin(), library.end(),
                       [&](const std::shared_ptr<Book>& book) {
                           bool titleMatches = title.isEmpty() || book->getTitle() == title;
                           bool authorMatches = author.isEmpty() || book->getAuthor() == author;
                           return titleMatches && authorMatches;
                       }),
        library.end()
        );
}

void Library::showLibrary() {
    QString list = "";

    for (int i = 0; i < library.size(); ++i) {
        list += library[i]->getTitle() + " "
        + library[i]->getAuthor() + " "
        + library[i]->getYear() + "\n";
    }

    QMessageBox::information(nullptr,
                             "Library",
                             list,
                             QMessageBox::Ok);
}

void Library::addTwoBooksAndShowLibrary(std::shared_ptr<Book> book1,
                                        std::shared_ptr<Book> book2) {
    addToLibrary(book1);
    addToLibrary(book2);
    showLibrary();
}

std::weak_ptr<Book> Library::findBook(QString title, QString author) {
    for (int i = 0; i < library.size(); ++i) {
        if ((library[i]->getTitle() == title && library[i]->getAuthor() == "")
            ||(library[i]->getTitle() == "" && library[i]->getAuthor() == author)
            || (library[i]->getTitle() == title && library[i]->getAuthor() == author)) {
            return library[i];
        }
    }

    return {};
}

void Library::findBookAndShowInfo(QString title, QString author) {
    if(findBook(title, author).lock()) {
    QString info = QString("Title: %1\nAuthor: %2\nYear: %3")
        .arg(findBook(title, author).lock()->getTitle())
        .arg(findBook(title, author).lock()->getAuthor())
        .arg(findBook(title, author).lock()->getYear());

    QMessageBox::information(nullptr,
                             "Book Info",
                             info,
                             QMessageBox::Ok);
    } else {
        QMessageBox::warning(
            nullptr,
            "Error",
            "Book wasn't found!"
            );
    }
}

void Library::addUser(std::shared_ptr<User> user) {
    userBookList[user];
}

void Library::addUser(QString name, QString id) {
    userBookList[std::make_shared<User>(name, id)];
}

void Library::deleteUser(std::shared_ptr<User> user) {
    userBookList.erase(user);
}

void Library::deleteUser(QString name, QString id) {
    auto it = userBookList.begin();
    while (it != userBookList.end()) {
        bool nameMatch = name.isEmpty() || it->first->getName() == name;
        bool idMatch = id.isEmpty() || it->first->getID() == id;
        if (nameMatch && idMatch) {
            it = userBookList.erase(it);
        } else {
            ++it;
        }
    }
}

const std::vector<std::shared_ptr<Book>>& Library::getBooks() const {
    return library;
}

const std::map<std::shared_ptr<User>,
               std::vector<std::unique_ptr<Book>>>&Library::getUsers() const {
    return userBookList;
}

bool Library::borrowBook(QString userId, QString bookTitle, QString bookAuthor) {
    std::shared_ptr<User> user = nullptr;
    for (const auto& [u, books] : userBookList) {
        if (u->getID() == userId) {
            user = u;
            break;
        }
    }
    if (!user) return false;

    auto bookIt = std::find_if(library.begin(), library.end(),
                               [&](const std::shared_ptr<Book>& b) {
                                   return b->getTitle() == bookTitle && b->getAuthor() == bookAuthor;
                               });

    if (bookIt == library.end()) return false;

    userBookList[user].push_back(std::make_unique<Book>(**bookIt));
    library.erase(bookIt);

    return true;
}

bool Library::returnBook(QString userId, QString bookTitle, QString bookAuthor) {
    std::shared_ptr<User> user = nullptr;
    for (const auto& [u, books] : userBookList) {
        if (u->getID() == userId) {
            user = u;
            break;
        }
    }
    if (!user) return false;

    auto& userBooks = userBookList[user];
    auto bookIt = std::find_if(userBooks.begin(), userBooks.end(),
                               [&](const std::unique_ptr<Book>& b) {
                                   return b->getTitle() == bookTitle && b->getAuthor() == bookAuthor;
                               });

    if (bookIt == userBooks.end()) return false;

    library.push_back(std::make_shared<Book>((*bookIt)->getTitle(),
                                             (*bookIt)->getAuthor(),
                                             (*bookIt)->getYear()));

    return true;
}

