#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QDesktopServices>

#pragma once

class UserGuide : public QDialog
{
    public:
        explicit UserGuide(QWidget *parent = nullptr);
        void updateText();
        void updateButton();
        void updateTitle();
    private slots:
        void close();
        void back();
        void next();
    private:
        QLabel* guideLabel;
        QPushButton* closeButton;
        QPushButton* backButton;
        QPushButton* nextButton;
        QVBoxLayout* layout;
        int index = 0;
};


