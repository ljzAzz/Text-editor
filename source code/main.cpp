#include "head.h"

void MyTextEdit::contextMenuEvent(QContextMenuEvent* event){
    QMenu* menu = QTextEdit::createStandardContextMenu();

    menu->actions().at(0)->setText(QString::fromUtf8("撤销(&Z)"));
    menu->actions().at(1)->setText(QString::fromUtf8("重做(&Y)"));
    menu->actions().at(3)->setText(QString::fromUtf8("剪切(&T)"));
    menu->actions().at(4)->setText(QString::fromUtf8("复制(&C)"));
    menu->actions().at(5)->setText(QString::fromUtf8("粘贴(&V)"));
    menu->actions().at(6)->setText(QString::fromUtf8("删除"));
    menu->actions().at(8)->setText(QString::fromUtf8("全选(&A)"));
    menu->exec(event->globalPos());
    delete menu;
}
TextEditor::TextEditor(QWidget* parent) : QMainWindow(parent) {
    QIcon icon;
    icon.addPixmap(QPixmap("D:\\Qt_Project\\untitled\\2.png"));
    this->setWindowIcon(icon);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), QApplication::primaryScreen()->availableGeometry()));
    this->setWindowTitle(QString::fromUtf8("文本编辑器"));
    this->text = new MyTextEdit(this);
    QFont font("Courier", 12);
    this->text->setFont(font);
    this->setCentralWidget(this->text);
    this->creatActions();
}
void TextEditor::creatActions(){
    this->openAct = new QAction(QString::fromUtf8("打开(&O)"),this);
    this->openAct->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_O));
    this->saveAct = new QAction(QString::fromUtf8("保存(&S)"),this);
    this->saveAct->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_S));
    this->findAct = new QAction(QString::fromUtf8("查找(&F)"),this);
    this->findAct->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_F));
    this->exitAct = new QAction(QString::fromUtf8("退出(&X)"),this);
    this->exitAct->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_X));
    this->TextEditor::creatMenu();
}
void TextEditor::creatMenu(){
    this->menu = this->menuBar()->addMenu(QString::fromUtf8("菜单"));
    this->menu->addAction(this->openAct);
    this->menu->addAction(this->saveAct);
    this->menu->addAction(this->findAct);
    this->menu->addSeparator();
    this->menu->addAction(this->exitAct);
    connect(this->openAct,&QAction::triggered,this,&TextEditor::textOpen);
    connect(this->saveAct,&QAction::triggered,this,&TextEditor::textSave);
    connect(this->findAct,&QAction::triggered,this,&TextEditor::findInit);
    connect(this->exitAct,&QAction::triggered,this,&QWidget::close);
}
void TextEditor::textOpen(){
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择文件"),"",QString::fromUtf8("文件类型 (*.txt)"));
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream in(&file);
            this->text->setPlainText(in.readAll());
            file.close();
        }
    }
    else{
        ERROR_SEND();
    }
}

void TextEditor::textSave(){
    QString filePath = QFileDialog::getSaveFileName(this,QString::fromUtf8("保存文件"),"",QString::fromUtf8("文件类型 (*.txt)"));
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream out(&file);
            out<<this->text->toPlainText();
            file.close();
        }
    }
    else{
        ERROR_SEND();
    }
}
void TextEditor::findInit() {
    if (!this->findDialog) {
        this->findDialog = new QDialog(this);
        this->findDialog->setWindowTitle(QString::fromUtf8("查找"));
        this->findString = new QLineEdit(this->findDialog);
        this->layout = new QVBoxLayout(this->findDialog);
        layout->addWidget(findString);
        this->findButton = new QPushButton(QString::fromUtf8("查找下一个"), this->findDialog);
        layout->addWidget(findButton);
        this->findString->setPlaceholderText(QString::fromUtf8("不支持正则表达式~"));
        connect(this->findButton, &QPushButton::clicked, this, &TextEditor::textFind);
        connect(this->findString, &QLineEdit::returnPressed, this, &TextEditor::textFind);
        QVBoxLayout* resultLayout = new QVBoxLayout();
        this->resultLabel = new QLabel(this->findDialog);
        QPushButton* closeButton = new QPushButton(QString::fromUtf8("关闭"), this->findDialog);
        resultLayout->addWidget(closeButton);
        resultLayout->addWidget(this->resultLabel);
        connect(closeButton, &QPushButton::clicked, this->findDialog, &QDialog::close);
        layout->addLayout(resultLayout);
    }

    this->findDialog->show();
    this->findString->setFocus();
    this->findString->selectAll();
}

void TextEditor::textFind() {
    QString fs = this->findString->text();
    QTextDocument* document = this->text->document();
    QTextCursor startCursor = this->text->textCursor();
    QTextCursor currentCursor = document->find(fs, startCursor);

    if (!currentCursor.isNull()) {
        this->text->setTextCursor(currentCursor);
        this->resultLabel->clear();
    } else {
        QTextCursor start(document);
        start.movePosition(QTextCursor::Start);
        QTextCursor foundCursor = document->find(fs, start);

        if (!foundCursor.isNull()) {
            this->text->setTextCursor(foundCursor);
            this->resultLabel->clear();
        } else {
            this->resultLabel->setText(QString::fromUtf8("找不到 '%1'").arg(fs));
        }
    }
}

void TextEditor::ERROR() {
    connect(this, &TextEditor::FILE_ERROR, this, [this](){
        QMessageBox::critical(this, QString::fromUtf8("错误"), QString::fromUtf8("似乎出错了......"));
    });
}

TextEditor::~TextEditor(){
    delete menu;
    delete text;
    delete findDialog;
    delete openAct;
    delete saveAct;
    delete findAct;
    delete exitAct;
}

int main(int argc,char**argv){
    QApplication app(argc, argv);
    QString styleSheet = R"(
    * {
        backdrop-filter: blur(28px);
        box-shadow: inset 22px 22px 38px rgba(255, 255, 255, 0.5), inset 28px 28px 44px rgba(0, 0, 0, 0.3);
    }

    *:focus, *:hover {
        box-shadow: inset 26px 26px 42px rgba(255, 255, 255, 0.6), inset 32px 32px 52px rgba(0, 0, 0, 0.9);
        outline: none;
    }
)";
    app.setStyleSheet(styleSheet);
    TextEditor textedit;
    QSize screenSize=textedit.size();
    QPixmap pixmap("D:\\Qt_Project\\untitled\\2.png");
    QSplashScreen splash(pixmap.scaled(0.8*screenSize,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    splash.show();
    app.processEvents();

    textedit.show();
    QThread::msleep(500);
    splash.finish(&textedit);
    return app.exec();
}
