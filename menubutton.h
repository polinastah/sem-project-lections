#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

class MenuButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MenuButton(const QString& text = "", QWidget* parent = nullptr);
    ~MenuButton() = default;

signals:
    void addUserRequested(const QString& name, const QString& id);
    void addBookRequested(const QString& title, const QString& author, const QString& year);
    void deleteUserRequested(const QString& name, const QString& id);
    void deleteBookRequested(const QString& title, const QString& author);
    void findUserRequested(const QString& name, const QString& id);
    void findBookRequested(const QString& title, const QString& author);

private slots:
    void showMenuDialog();
    void showAddUserDialog();
    void showAddBookDialog();
    void showDeleteUserDialog();
    void showDeleteBookDialog();
    void showFindUserDialog();
    void showFindBookDialog();

private:
    QString text;
    QDialog* createFormDialog(const QString& title, const QStringList& fields);

    QDialog* menuDialog;
};

#endif // MENUBUTTON_H
