#include "book.h"

Book::Book(QString title_, QString author_, QString year_) :
    title(title_), author(author_), year(year_)
{}

QString Book::getTitle() {
    return title;
}

QString Book::getAuthor() {
    return author;
}

QString Book::getYear(){
    return year;
}

void showBookInfo(std::unique_ptr<Book> book) {
    QString info = QString("Title: %1\nAuthor: %2\nYear: %3")
    .arg(book->getTitle())
        .arg(book->getAuthor())
        .arg(book->getYear());

    QMessageBox::information(nullptr,
                             "Book Info",
                             info,
                             QMessageBox::Ok);
}

void createBook(QString title, QString author, QString year) {
    auto book = std::make_unique<Book>(title, author, year);
    showBookInfo(std::move(book));
}
