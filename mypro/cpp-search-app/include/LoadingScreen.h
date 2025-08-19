#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QLinearGradient>
#include <QFontDatabase>

class LoadingScreen : public QWidget {
    Q_OBJECT

public:
    explicit LoadingScreen(QWidget *parent = nullptr);
    ~LoadingScreen();

    void startLoading();
    void setProgress(int value);
    void setStatus(const QString& status);

signals:
    void loadingFinished();

private slots:
    void updateProgress();
    void onLoadingComplete();

private:
    void setupUI();
    void setupAnimations();
    void paintEvent(QPaintEvent* event) override;
    
    // UI Components
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QLabel* m_subtitleLabel;
    QLabel* m_statusLabel;
    QProgressBar* m_progressBar;
    QLabel* m_iconLabel;
    
    // Animation components
    QTimer* m_progressTimer;
    QTimer* m_animationTimer;
    QPropertyAnimation* m_fadeInAnimation;
    QPropertyAnimation* m_progressAnimation;
    QGraphicsOpacityEffect* m_opacityEffect;
    
    // Animation state
    int m_currentProgress;
    int m_targetProgress;
    bool m_isLoading;
    
    // Custom styling
    void applyCustomStyle();
    void loadCustomFonts();
};
