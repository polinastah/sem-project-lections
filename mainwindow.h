#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QScreen>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

#include "menubutton.h"
#include "book.h"
#include "library.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    int width;
    int height;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTableView *userTable;
    QStandardItemModel *userTableModel;
    QTableView *booksTable;
    QStandardItemModel *booksTableModel;
    MenuButton *menuButton;
    Library library; // Объект Library

    void setupUserTable();
    void setupBooksTable();
    void setupLayout();
    void updateBooksTable();
    void updateUsersTable();
    void showBorrowBookDialog(const QString& userId);

private slots:
    void handleAddBookRequested(const QString& title,
                                const QString& author, const QString& year);
    void handleDeleteBookRequested(const QString& title, const QString& author);

    void handleAddUserRequested(const QString& name, const QString& id);
    void handleDeleteUserRequested(const QString& name, const QString& id);

    void handleFindUserRequested(const QString& name, const QString& id);
    void handleFindBookRequested(const QString& title, const QString& author);

    void handleUserTableClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
