#include "MainWindow.h"
#include "ResultsWidget.h"
#include "SearchHistory.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_searchInProgress(false) {
    qDebug() << "MainWindow: ctor start";
    // Loading screen disabled for stability
    qDebug() << "MainWindow: loading screen skipped";
    // Initialize core components
    m_searchHistory = new SearchHistory(this);
    m_offlineQA = new OfflineQADatabase(this);
    qDebug() << "MainWindow: after core components";
    setupUI();
    qDebug() << "MainWindow: after setupUI";
    setupMenus();
    qDebug() << "MainWindow: after setupMenus";
    setupAnimations();
    qDebug() << "MainWindow: after setupAnimations";
    m_themeMode = "Beast";
    applyFuturisticStyle();
    qDebug() << "MainWindow: after applyFuturisticStyle";
    
    // Connect signals
    connect(m_resultsWidget, &ResultsWidget::resultClicked,
            this, &MainWindow::onResultClicked);
    
    loadSettings();
    qDebug() << "MainWindow: ctor end";
}

MainWindow::~MainWindow() {
    saveSettings();
}

void MainWindow::setupUI() {
    qDebug() << "setupUI: start";
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Top navigation + search controls in a styled top bar
    m_topBar = new QFrame(this);
    m_topBar->setObjectName("TopBar");
    m_topBar->setFrameShape(QFrame::NoFrame);
    m_topBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_topBar->setFixedHeight(54);
    m_searchLayout = new QHBoxLayout(m_topBar);
    m_searchLayout->setContentsMargins(12, 8, 12, 8);
    m_searchLayout->setSpacing(10);

    // Logo
    m_logoLabel = new QLabel(this);
    m_logoLabel->setText("🧠 Imilya Minds");
    m_logoLabel->setStyleSheet("font-weight: 800; font-size: 16px; color: #00d4ff;");

    m_backButton = new QPushButton("◀", this);
    m_forwardButton = new QPushButton("▶", this);
    m_homeButton = new QPushButton("⌂", this);
    m_themeToggleButton = new QPushButton("Beast", this);
    QPushButton* copyButton = new QPushButton("Copy", this);
    m_backButton->setToolTip("Back");
    m_forwardButton->setToolTip("Forward");
    m_homeButton->setToolTip("Home");
    m_themeToggleButton->setToolTip("Toggle Theme");
    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::onBack);
    connect(m_forwardButton, &QPushButton::clicked, this, &MainWindow::onForward);
    connect(m_homeButton, &QPushButton::clicked, this, &MainWindow::onHome);
    connect(m_themeToggleButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::quickCopyAnswer);

    m_urlBar = new QLineEdit(this);
    m_urlBar->setPlaceholderText("offline://");
    m_urlBar->setReadOnly(true);
    
    m_searchInput = new QLineEdit(this);
    m_searchInput->setPlaceholderText("Enter search query...");
    m_searchInput->setMinimumHeight(30);
    connect(m_searchInput, &QLineEdit::returnPressed, this, &MainWindow::performSearch);
    connect(m_searchInput, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    // Fast, robust autocomplete using QCompleter
    m_completerModel = new QStringListModel(this);
    m_completer = new QCompleter(m_completerModel, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_searchInput->setCompleter(m_completer);
    
    m_searchButton = new QPushButton("Search", this);
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::performSearch);
    
    m_clearButton = new QPushButton("Clear", this);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::clearResults);
    
    m_searchLayout->addWidget(m_logoLabel);
    m_searchLayout->addWidget(m_backButton);
    m_searchLayout->addWidget(m_forwardButton);
    m_searchLayout->addWidget(m_homeButton);
    m_searchLayout->addWidget(m_urlBar, /*stretch*/1);
    m_searchLayout->addWidget(m_searchInput);
    m_searchLayout->addWidget(m_searchButton);
    m_searchLayout->addWidget(m_clearButton);
    m_searchLayout->addWidget(copyButton);
    m_searchLayout->addWidget(m_themeToggleButton);
    
    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    
    // Status label
    m_statusLabel = new QLabel("Ready to search", this);
    
    // Results area: full-width under the search bar
    m_resultsWidget = new ResultsWidget(this);
    
    // Add to main layout
    m_mainLayout->addWidget(m_topBar);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addWidget(m_statusLabel);
    m_mainLayout->addWidget(m_resultsWidget, 1);
    
    // Window properties
    setWindowTitle("Search & Browse - C++ Search App");
    setMinimumSize(800, 600);
    resize(1000, 700);
    qDebug() << "setupUI: end";
}

void MainWindow::setupMenus() {
    qDebug() << "setupMenus: start";
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Settings", this, &MainWindow::showSettings);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);
    
    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("&Clear Results", this, &MainWindow::clearResults);
    editMenu->addAction("Clear &History", this, [this]() {
        m_searchHistory->clearHistory();
        statusBar()->showMessage("Search history cleared", 2000);
    });
    
    // View menu
    QMenu* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Show &History", this, &MainWindow::showSearchHistory);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::showAbout);
    
    // Status bar
    statusBar()->showMessage("Ready");
    qDebug() << "setupMenus: end";
}

void MainWindow::performSearch() {
    QString query = m_searchInput->text().trimmed();
    if (query.isEmpty()) {
        statusBar()->showMessage("Please enter a search query", 2000);
        return;
    }
    
    if (m_searchInProgress) {
        statusBar()->showMessage("Search already in progress", 2000);
        return;
    }
    
    // Clear completer popup
    if (m_completer) {
        m_completerModel->setStringList({});
    }
    
    m_currentQuery = query;
    m_searchHistory->addSearch(query);
    
    setSearchInProgress(true);
    
    // Offline-only Q&A search
    statusBar()->showMessage(QString("🔎 Looking up offline Q&A for '%1'...").arg(query), 0);
    m_progressBar->setRange(0, 0); // Indeterminate progress
    
    QVector<SearchResult> results = buildOfflineResults(query);

    // Mimic async completion
    QTimer::singleShot(150, this, [this, results]() {
        onSearchFinished(results);
        if (!results.isEmpty()) {
            navigateTo(results.first());
        }
    });
    
    // Animate search button
    if (m_searchAnimation) {
        QRect currentGeometry = m_searchButton->geometry();
        QRect newGeometry = currentGeometry.adjusted(-2, -2, 2, 2);
        m_searchAnimation->setStartValue(currentGeometry);
        m_searchAnimation->setEndValue(newGeometry);
        m_searchAnimation->start();
    }
}

void MainWindow::onSearchFinished(const QVector<SearchResult>& results) {
    setSearchInProgress(false);
    
    // Keep UI snappy: skip heavy animations
    
    m_resultsWidget->displayResults(results);
    
    // Enhanced status message
    QString message;
    if (results.isEmpty()) {
        message = QString("❌ No results found for '%1'").arg(m_currentQuery);
    } else if (results.size() == 1) {
        message = QString("✅ Found 1 result for '%1'").arg(m_currentQuery);
    } else {
        message = QString("✅ Found %1 results for '%2'").arg(results.size()).arg(m_currentQuery);
    }
    
    statusBar()->showMessage(message, 5000);
    
    // Show results count in title
    if (!results.isEmpty()) {
        setWindowTitle(QString("Quantum Search - %1 results for '%2'").arg(results.size()).arg(m_currentQuery));
    }
}

void MainWindow::onResultClicked(const SearchResult& result) {
    navigateTo(result);
}

void MainWindow::onSearchTextChanged(const QString& text) {
    if (text.length() <= 1) {
        m_completerModel->setStringList({});
        return;
    }
    QStringList suggestions = m_offlineQA->getSuggestions(text);
    onSuggestionsReady(suggestions);
}

void MainWindow::onSuggestionsReady(const QStringList& suggestions) {
    m_completerModel->setStringList(suggestions);
}

void MainWindow::clearResults() {
    m_resultsWidget->clearResults();
    statusBar()->showMessage("Results cleared", 2000);
}

void MainWindow::showSearchHistory() {
    QStringList history = m_searchHistory->getRecentSearches(20);
    QString historyText = history.join("\n");
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Search History");
    msgBox.setText("Recent searches:");
    msgBox.setDetailedText(historyText);
    msgBox.exec();
}

// onEngineChanged removed in offline-only mode

void MainWindow::showSettings() {
    QMessageBox::information(this, "Settings", "Settings dialog not implemented yet.");
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About Search & Browse",
        "Search & Browse v1.0\n\n"
        "A desktop search application with web scraping capabilities.\n"
        "Built with C++ and Qt6.\n\n"
        "Features:\n"
        "• Multiple search engines\n"
        "• Browser integration\n"
        "• Search history\n"
        "• Cross-platform support");
}

void MainWindow::setSearchInProgress(bool inProgress) {
    m_searchInProgress = inProgress;
    m_searchButton->setEnabled(!inProgress);
    m_progressBar->setVisible(inProgress);
    
    if (inProgress) {
        m_progressBar->setRange(0, 0); // Indeterminate progress
    }
}

void MainWindow::saveSettings() {
    // Implementation would save window geometry, selected engine, etc.
}

void MainWindow::loadSettings() {
    // Implementation would load saved settings
}

// Stub implementations
void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    Q_UNUSED(reason)
}

void MainWindow::toggleWindowVisibility() {
    setVisible(!isVisible());
}

void MainWindow::setupAutoComplete() {}

void MainWindow::showLoadingScreen() {
    m_loadingScreen = new LoadingScreen(this);
    connect(m_loadingScreen, &LoadingScreen::loadingFinished, this, [this]() {
        show();
        activateWindow();
    });
    m_loadingScreen->startLoading();
}

void MainWindow::setupAnimations() {
    qDebug() << "setupAnimations: start";
    // Search button animation
    m_searchAnimation = new QPropertyAnimation(m_searchButton, "geometry");
    m_searchAnimation->setDuration(200);
    m_searchAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // Results fade in animation
    m_resultsOpacityEffect = new QGraphicsOpacityEffect(m_resultsWidget);
    m_resultsWidget->setGraphicsEffect(m_resultsOpacityEffect);
    m_resultsOpacityEffect->setOpacity(0.0);
    
    m_resultsAnimation = new QPropertyAnimation(m_resultsOpacityEffect, "opacity");
    m_resultsAnimation->setDuration(500);
    m_resultsAnimation->setStartValue(0.0);
    m_resultsAnimation->setEndValue(1.0);
    m_resultsAnimation->setEasingCurve(QEasingCurve::OutCubic);
    qDebug() << "setupAnimations: end";
}

void MainWindow::applyFuturisticStyle() {
    if (m_themeMode == "Best") {
        applyBestStyle();
        return;
    }
    applyBeastStyle();
}

void MainWindow::applyBestStyle() {
    setStyleSheet(R"(
        QMainWindow {
            background: #f6f7fb;
            color: #111111;
        }
        #TopBar {
            background: #ffffff;
            border-bottom: 1px solid #e1e4ea;
        }
        QLineEdit {
            background-color: #ffffff;
            border: 2px solid #e1e4ea;
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 14px;
            color: #111111;
            selection-background-color: #4a90e2;
        }
        QLineEdit:focus { border-color: #4a90e2; }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a90e2, stop:1 #357ab8);
            border: none;
            border-radius: 10px;
            padding: 10px 18px;
            font-size: 14px;
            font-weight: 600;
            color: #ffffff;
            min-height: 20px;
            box-shadow: 0px 2px 6px rgba(0,0,0,0.15);
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5aa0f2, stop:1 #468bcd);
        }
        QPushButton:disabled { background-color: #c9ced8; color: #8c96a8; }
        QProgressBar { border: none; border-radius: 4px; background-color: #e9edf3; text-align: center; height: 8px; }
        QProgressBar::chunk { background: #4a90e2; border-radius: 4px; }
        QLabel { color: #333333; font-size: 13px; }
        QSplitter::handle { background-color: #d9dde5; border-radius: 2px; }
        QTextBrowser {
            background-color: #ffffff;
            border: 2px solid #e1e4ea;
            border-radius: 12px;
            padding: 16px;
            color: #222222;
            font-size: 14px;
            line-height: 1.5;
        }
        QMenuBar { background-color: #ffffff; color: #111111; border-bottom: 1px solid #e1e4ea; }
        QMenuBar::item { background-color: transparent; padding: 8px 12px; }
        QMenuBar::item:selected { background-color: #4a90e2; color: #ffffff; }
        QMenu { background-color: #ffffff; border: 1px solid #e1e4ea; border-radius: 8px; padding: 4px; }
        QMenu::item { padding: 8px 20px; color: #111111; }
        QMenu::item:selected { background-color: #4a90e2; color: #ffffff; }
        QStatusBar { background-color: #ffffff; color: #333333; border-top: 1px solid #e1e4ea; }
    )");
    setWindowTitle("Imilya Minds - Best Mode");
    if (m_resultsWidget) m_resultsWidget->applyLightStyle();
}

void MainWindow::applyBeastStyle() {
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            color: #ffffff;
        }
        #TopBar {
            background: #141414;
            border-bottom: 1px solid #2a2a2a;
        }
        
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #333333;
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 14px;
            color: #ffffff;
            selection-background-color: #00d4ff;
        }
        
        QLineEdit:focus {
            border-color: #00d4ff;
            background-color: #333333;
        }
        
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00d4ff, stop:1 #0099ff);
            border: none;
            border-radius: 10px;
            padding: 10px 18px;
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
            min-height: 20px;
            box-shadow: 0px 2px 8px rgba(0, 212, 255, 0.2);
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00e6ff, stop:1 #00aaff);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0099ff, stop:1 #0077cc);
        }
        
        QPushButton:disabled {
            background-color: #555555;
            color: #888888;
        }
        
        QComboBox {
            background-color: #2a2a2a;
            border: 2px solid #333333;
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 14px;
            color: #ffffff;
            min-height: 20px;
        }
        
        QComboBox:focus {
            border-color: #00d4ff;
        }
        
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #ffffff;
            margin-right: 5px;
        }
        
        QProgressBar {
            border: none;
            border-radius: 4px;
            background-color: #2a2a2a;
            text-align: center;
            height: 8px;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:0.5 #0099ff, stop:1 #00d4ff);
            border-radius: 4px;
        }
        
        QLabel {
            color: #cccccc;
            font-size: 13px;
        }
        
        QSplitter::handle {
            background-color: #333333;
            border-radius: 2px;
        }
        
        QSplitter::handle:hover {
            background-color: #00d4ff;
        }
        
        QTextBrowser {
            background-color: #2a2a2a;
            border: 2px solid #333333;
            border-radius: 12px;
            padding: 16px;
            color: #ffffff;
            font-size: 14px;
            line-height: 1.55;
        }
        
        QMenuBar {
            background-color: #1a1a1a;
            color: #ffffff;
            border-bottom: 1px solid #333333;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 12px;
        }
        
        QMenuBar::item:selected {
            background-color: #00d4ff;
            color: #000000;
        }
        
        QMenu {
            background-color: #2a2a2a;
            border: 1px solid #333333;
            border-radius: 8px;
            padding: 4px;
        }
        
        QMenu::item {
            padding: 8px 20px;
            color: #ffffff;
        }
        
        QMenu::item:selected {
            background-color: #00d4ff;
            color: #000000;
        }
        
        QStatusBar {
            background-color: #1a1a1a;
            color: #cccccc;
            border-top: 1px solid #333333;
        }
    )");
    setWindowTitle("Imilya Minds - Beast Mode");
    setMinimumSize(1000, 700);
    resize(1200, 800);
    if (m_resultsWidget) m_resultsWidget->applyDarkStyle();
}

void MainWindow::navigateTo(const SearchResult& result) {
    QString descHtml = result.description;
    descHtml.replace('\n', "<br/>");
    // Display a compact banner in status bar instead of side preview
    statusBar()->showMessage(QString("%1 — %2").arg(result.title, result.displayUrl), 5000);
    updateUrlBar(result);

    // Manage history
    if (m_historyIndex >= 0 && m_historyIndex < m_navigationHistory.size()-1) {
        m_navigationHistory = m_navigationHistory.mid(0, m_historyIndex + 1);
    }
    m_navigationHistory.append(result);
    m_historyIndex = m_navigationHistory.size() - 1;

    m_backButton->setEnabled(m_historyIndex > 0);
    m_forwardButton->setEnabled(false);
}

void MainWindow::quickCopyAnswer() {
    if (m_navigationHistory.isEmpty()) return;
    const SearchResult& r = m_navigationHistory.last();
    QClipboard* cb = QApplication::clipboard();
    if (!cb) return;
    cb->setText(QString("%1\n%2").arg(r.title, r.description));
    statusBar()->showMessage("Answer copied to clipboard", 2000);
}

void MainWindow::updateUrlBar(const SearchResult& result) {
    QString urlText = result.url.toString();
    if (!result.title.isEmpty()) {
        urlText += " — " + result.title;
    }
    m_urlBar->setText(urlText);
}

void MainWindow::onBack() {
    if (m_historyIndex > 0) {
        m_historyIndex--;
        const SearchResult& r = m_navigationHistory.at(m_historyIndex);
        statusBar()->showMessage(QString("%1 — %2").arg(r.title, r.displayUrl), 5000);
        updateUrlBar(r);
        m_backButton->setEnabled(m_historyIndex > 0);
        m_forwardButton->setEnabled(m_historyIndex < m_navigationHistory.size() - 1);
    }
}

void MainWindow::onForward() {
    if (m_historyIndex < m_navigationHistory.size() - 1) {
        m_historyIndex++;
        const SearchResult& r = m_navigationHistory.at(m_historyIndex);
        statusBar()->showMessage(QString("%1 — %2").arg(r.title, r.displayUrl), 5000);
        updateUrlBar(r);
        m_backButton->setEnabled(m_historyIndex > 0);
        m_forwardButton->setEnabled(m_historyIndex < m_navigationHistory.size() - 1);
    }
}

void MainWindow::onHome() {
    m_urlBar->setText("offline://home");
}

void MainWindow::toggleTheme() {
    if (m_themeMode == "Beast") {
        m_themeMode = "Best";
        m_themeToggleButton->setText("Best");
    } else {
        m_themeMode = "Beast";
        m_themeToggleButton->setText("Beast");
    }
    applyFuturisticStyle();
}

QVector<SearchResult> MainWindow::buildOfflineResults(const QString& query) const {
    QVector<SearchResult> results;
    if (m_offlineQA->hasOfflineAnswer(query)) {
        SearchResult answer = m_offlineQA->getOfflineAnswer(query);
        if (answer.isValid()) {
            results.append(answer);
        }
    }
    // Broaden with suggestions
    QStringList suggestions = m_offlineQA->getSuggestions(query);
    for (const QString& s : suggestions) {
        SearchResult r = m_offlineQA->getOfflineAnswer(s);
        if (r.isValid()) {
            results.append(r);
        }
    }
    // If still empty, include more from catalog
    if (results.isEmpty()) {
        QVector<SearchResult> all = m_offlineQA->getAllOfflineAnswers();
        int limit = qMin(25, all.size());
        for (int i = 0; i < limit; ++i) {
            results.append(all.at(i));
        }
    }
    return results;
}

#include "MainWindow.moc"
