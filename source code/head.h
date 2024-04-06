#ifndef HEAD_H
#define HEAD_H
#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QSize>
#include <QScreen>
#include <QStyle>
#include <QThread>
#include <QWidget>
#include <QKeySequence>
#include <QMetaObject>
#include <QObject>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QLabel>
#include <QTextBlock>
class MyTextEdit:public QTextEdit{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget* parent):QTextEdit(parent){};
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};


class TextEditor : public QMainWindow {
    Q_OBJECT
public:
    TextEditor(QWidget* parent=nullptr);
    virtual ~TextEditor();

private:
    QMenu* menu;
    MyTextEdit* text;
    QDialog* findDialog;
    QLineEdit* findString;
    QPushButton* findButton;
    QVBoxLayout *layout;
    QLabel* resultLabel;
    void textOpen();
    void textSave();
    void findInit();
    void textFind();
    void creatMenu();
    void creatActions();
    void ERROR();
    QAction* openAct;
    QAction* saveAct;
    QAction* findAct;
    QAction* exitAct;

private slots:
    void ERROR_SEND() {
        emit FILE_ERROR();
    }

signals:
    void FILE_ERROR();
};



#endif // HEAD_H
