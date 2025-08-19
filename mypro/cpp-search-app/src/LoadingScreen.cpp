#include "LoadingScreen.h"
#include <QApplication>
#include <QScreen>
#include <QPainterPath>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QDir>

LoadingScreen::LoadingScreen(QWidget *parent)
    : QWidget(parent)
    , m_currentProgress(0)
    , m_targetProgress(0)
    , m_isLoading(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupUI();
    setupAnimations();
    applyCustomStyle();
    loadCustomFonts();
    
    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

LoadingScreen::~LoadingScreen() {
    if (m_progressTimer) m_progressTimer->stop();
    if (m_animationTimer) m_animationTimer->stop();
}

void LoadingScreen::setupUI() {
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(40, 40, 40, 40);
    
    // Icon label with placeholder
    m_iconLabel = new QLabel("🔍");
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setStyleSheet("font-size: 48px; margin-bottom: 10px;");
    
    // Title label
    m_titleLabel = new QLabel("Quantum Search");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #00d4ff; margin-bottom: 5px;");
    
    // Subtitle label
    m_subtitleLabel = new QLabel("Advanced Search & Discovery Engine");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet("font-size: 16px; color: #888888; margin-bottom: 20px;");
    
    // Status label
    m_statusLabel = new QLabel("Initializing...");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size: 14px; color: #aaaaaa; margin-bottom: 15px;");
    
    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(8);
    
    // Add widgets to layout
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_iconLabel);
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_subtitleLabel);
    m_mainLayout->addWidget(m_statusLabel);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addStretch();
    
    // Set fixed size
    setFixedSize(500, 400);
    
    // Apply drop shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 10);
    setGraphicsEffect(shadow);
}

void LoadingScreen::setupAnimations() {
    // Fade in animation
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(0.0);
    
    m_fadeInAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_fadeInAnimation->setDuration(800);
    m_fadeInAnimation->setStartValue(0.0);
    m_fadeInAnimation->setEndValue(1.0);
    m_fadeInAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // Progress animation
    m_progressAnimation = new QPropertyAnimation(m_progressBar, "value");
    m_progressAnimation->setDuration(200);
    m_progressAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // Progress timer
    m_progressTimer = new QTimer(this);
    m_progressTimer->setInterval(50);
    connect(m_progressTimer, &QTimer::timeout, this, &LoadingScreen::updateProgress);
    
    // Animation timer for smooth progress updates
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100);
    connect(m_animationTimer, &QTimer::timeout, this, &LoadingScreen::onLoadingComplete);
}

void LoadingScreen::applyCustomStyle() {
    // Custom progress bar style
    m_progressBar->setStyleSheet(R"(
        QProgressBar {
            border: none;
            border-radius: 4px;
            background-color: #2a2a2a;
            text-align: center;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:0.5 #0099ff, stop:1 #00d4ff);
            border-radius: 4px;
        }
    )");
    
    // Set background style
    setStyleSheet(R"(
        LoadingScreen {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            border: 1px solid #333333;
            border-radius: 15px;
        }
    )");
}

void LoadingScreen::loadCustomFonts() {
    // Try to load custom fonts if available
    QString fontDir = QApplication::applicationDirPath() + "/resources/fonts/";
    if (QDir(fontDir).exists()) {
        QFontDatabase::addApplicationFont(fontDir + "OpenSans-Regular.ttf");
        QFontDatabase::addApplicationFont(fontDir + "OpenSans-Bold.ttf");
        
        QFont titleFont("Open Sans", 32, QFont::Bold);
        QFont subtitleFont("Open Sans", 16);
        QFont statusFont("Open Sans", 14);
        
        m_titleLabel->setFont(titleFont);
        m_subtitleLabel->setFont(subtitleFont);
        m_statusLabel->setFont(statusFont);
    }
}

void LoadingScreen::startLoading() {
    m_isLoading = true;
    m_currentProgress = 0;
    m_targetProgress = 0;
    
    // Start fade in
    m_fadeInAnimation->start();
    
    // Start progress simulation
    m_progressTimer->start();
    
    // Simulate loading steps
    QTimer::singleShot(500, [this]() { setStatus("Loading search engines..."); });
    QTimer::singleShot(1500, [this]() { setStatus("Initializing network..."); });
    QTimer::singleShot(2500, [this]() { setStatus("Preparing UI components..."); });
    QTimer::singleShot(3500, [this]() { setStatus("Almost ready..."); });
    QTimer::singleShot(4000, [this]() { 
        setStatus("Launching application...");
        m_targetProgress = 100;
    });
}

void LoadingScreen::setProgress(int value) {
    m_targetProgress = value;
    if (!m_progressTimer->isActive()) {
        m_progressTimer->start();
    }
}

void LoadingScreen::setStatus(const QString& status) {
    m_statusLabel->setText(status);
    
    // Animate status change
    QPropertyAnimation* fadeOut = new QPropertyAnimation(m_statusLabel, "styleSheet");
    fadeOut->setDuration(150);
    fadeOut->setStartValue("color: #aaaaaa;");
    fadeOut->setEndValue("color: #00d4ff;");
    
    QPropertyAnimation* fadeIn = new QPropertyAnimation(m_statusLabel, "styleSheet");
    fadeIn->setDuration(150);
    fadeIn->setStartValue("color: #00d4ff;");
    fadeIn->setEndValue("color: #aaaaaa;");
    
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup;
    group->addAnimation(fadeOut);
    group->addAnimation(fadeIn);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoadingScreen::updateProgress() {
    if (m_currentProgress < m_targetProgress) {
        m_currentProgress += 2;
        m_progressBar->setValue(m_currentProgress);
        
        if (m_currentProgress >= m_targetProgress) {
            m_progressTimer->stop();
            m_animationTimer->start();
        }
    }
}

void LoadingScreen::onLoadingComplete() {
    m_animationTimer->stop();
    
    // Fade out animation
    QPropertyAnimation* fadeOut = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeOut->setDuration(500);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InCubic);
    
    connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
        emit loadingFinished();
        close();
    });
    
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoadingScreen::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background with gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(26, 26, 26));
    gradient.setColorAt(1, QColor(10, 10, 10));
    
    painter.fillRect(rect(), gradient);
    
    // Draw border
    painter.setPen(QPen(QColor(51, 51, 51), 1));
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 15, 15);
    
    // Draw accent line at top
    QLinearGradient accentGradient(0, 0, width(), 0);
    accentGradient.setColorAt(0, QColor(0, 212, 255));
    accentGradient.setColorAt(0.5, QColor(0, 153, 255));
    accentGradient.setColorAt(1, QColor(0, 212, 255));
    
    painter.setPen(QPen(accentGradient, 3));
    painter.drawLine(20, 20, width() - 20, 20);
}
