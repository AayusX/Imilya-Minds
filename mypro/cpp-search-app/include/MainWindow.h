#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QComboBox>
#include <QLabel>
#include <QSplitter>
#include <QTextBrowser>
#include <QListWidget>
#include <QCompleter>
#include <QStringListModel>
#include <QTabWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <memory>

// Include the actual header files instead of forward declarations
#include "SearchResult.h"
#include "ResultsWidget.h"
#include "SearchHistory.h"
#include "LoadingScreen.h"
#include "OfflineQADatabase.h"

/**
 * Main application window
 * Features:
 * - Search input with auto-complete
 * - Multiple search engines support
 * - Results display with preview
 * - Search history
 * - Browser integration
 * - System tray support
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void performSearch();
    void onSearchFinished(const QVector<struct SearchResult>& results);
    void onResultClicked(const struct SearchResult& result);
    void onSearchTextChanged(const QString& text);
    void onSuggestionsReady(const QStringList& suggestions);
    void clearResults();
    void onBack();
    void onForward();
    void onHome();
    void toggleTheme();
    void quickCopyAnswer();
    void showSearchHistory();
    void showSettings();
    void showAbout();
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleWindowVisibility();

private:
    void setupUI();
    void setupMenus();
    void setupSystemTray();
    void setupAutoComplete();
    void setupAnimations();
    void updateSearchSuggestions();
    void saveSettings();
    void loadSettings();
    void setSearchInProgress(bool inProgress);
    void applyFuturisticStyle();
    void showLoadingScreen();
    
    // UI Components
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_searchLayout;
    QFrame* m_topBar;
    QLabel* m_logoLabel;
    
    // Search controls
    QLineEdit* m_searchInput;
    QPushButton* m_searchButton;
    QPushButton* m_clearButton;
    QLineEdit* m_urlBar;
    QPushButton* m_backButton;
    QPushButton* m_forwardButton;
    QPushButton* m_homeButton;
    QPushButton* m_themeToggleButton;
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;
    
    // Results area
    ResultsWidget* m_resultsWidget;
    
    // Tabs
    QTabWidget* m_tabWidget;
    QListWidget* m_historyWidget;
    
    // System tray
    QSystemTrayIcon* m_systemTray;
    QMenu* m_trayMenu;
    
    // Core components
    SearchHistory* m_searchHistory;
    OfflineQADatabase* m_offlineQA;
    
    
    // Auto-complete
    QTimer* m_suggestionTimer;
    // Autocomplete
    QCompleter* m_completer;
    QStringListModel* m_completerModel;
    
    // Loading screen
    LoadingScreen* m_loadingScreen;
    
    // Animation components
    QPropertyAnimation* m_searchAnimation;
    QPropertyAnimation* m_resultsAnimation;
    QGraphicsOpacityEffect* m_resultsOpacityEffect;
    
    // State
    bool m_searchInProgress;
    QString m_currentQuery;
    QVector<SearchResult> m_navigationHistory;
    int m_historyIndex = -1;
    QString m_themeMode; // "Beast" (dark) or "Best" (light)
    
    // Settings
    QString m_defaultBrowser;
    bool m_enableAutoComplete;
    bool m_enableSystemTray;
    int m_maxResults;

    void navigateTo(const SearchResult& result);
    void updateUrlBar(const SearchResult& result);
    void applyBestStyle();
    void applyBeastStyle();
    QVector<SearchResult> buildOfflineResults(const QString& query) const;
};
