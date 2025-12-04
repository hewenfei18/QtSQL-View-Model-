#include "welcomeview.h"
#include <QVBoxLayout>
#include <QFont>

WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent)
{
    QLabel *titleLabel = new QLabel("医院诊疗系统");
    QFont font = titleLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);

    QPushButton *enterBtn = new QPushButton("进入系统");
    enterBtn->setFixedSize(150, 40);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(50);
    layout->addWidget(enterBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    // 连接按钮点击信号到槽函数
    connect(enterBtn, &QPushButton::clicked, this, &WelcomeView::onEnterClicked);
}

// 补充 onEnterClicked 槽函数的实现
void WelcomeView::onEnterClicked()
{
    // 触发信号，跳转到登录界面
    emit toLoginView();
}
