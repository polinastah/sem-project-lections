#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <memory>
#include <QMessageBox>

class Book
{
private:
    QString title;
    QString author;
    QString year;
public:
    Book(QString title_ = "", QString author_ = "", QString year_ = "");
    ~Book() = default;

    QString getTitle();
    QString getAuthor();
    QString getYear();
};

void showBookInfo(std::unique_ptr<Book> book);
void createBook(QString title, QString author, QString year);

#endif // BOOK_H
