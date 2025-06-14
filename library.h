#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <memory>
#include <QString>
#include <map>

#include "book.h"
#include "user.h"

class Library
{
private:
    std::vector<std::shared_ptr<Book>> library;
    std::map<std::shared_ptr<User>, std::vector<std::unique_ptr<Book>>> userBookList;

public:
    Library();
    ~Library() = default;

    void addToLibrary(std::shared_ptr<Book> book);
    void addToLibrary(QString title, QString author, QString year);

    void deleteFromLibrary(std::shared_ptr<Book> book);
    void deleteFromLibrary(QString title, QString author);

    void showLibrary();

    void addTwoBooksAndShowLibrary(std::shared_ptr<Book> book1, std::shared_ptr<Book> book2);

    std::weak_ptr<Book> findBook(QString title, QString author);
    void findBookAndShowInfo(QString title, QString author);

    void addUser(std::shared_ptr<User> user);
    void addUser(QString name, QString id);

    void deleteUser(std::shared_ptr<User> user);
    void deleteUser(QString name, QString id);

    const std::vector<std::shared_ptr<Book>>& getBooks() const;
    const std::map<std::shared_ptr<User>,
                   std::vector<std::unique_ptr<Book>>>& getUsers() const;

    bool borrowBook(QString userId, QString bookTitle, QString bookAuthor);
    bool returnBook(QString userId, QString bookTitle, QString bookAuthor);

};

#endif // LIBRARY_H
