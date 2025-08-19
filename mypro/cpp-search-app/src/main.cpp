#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QScreen>
#include <QFontDatabase>

#include "MainWindow.h"

void setupApplicationStyle() {
    // Set a modern dark theme
    qApp->setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(26, 26, 26));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(10, 10, 10));
    darkPalette.setColor(QPalette::AlternateBase, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(0, 212, 255));
    darkPalette.setColor(QPalette::ButtonText, Qt::black);
    darkPalette.setColor(QPalette::BrightText, QColor(0, 255, 255));
    darkPalette.setColor(QPalette::Link, QColor(0, 212, 255));
    darkPalette.setColor(QPalette::Highlight, QColor(0, 212, 255));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    
    qApp->setPalette(darkPalette);
}

void ensureDataDirectory() {
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataDir)) {
        dir.mkpath(dataDir);
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Imilya Minds");
    app.setApplicationVersion("1.0.0");
    app.setApplicationDisplayName("Imilya Minds");
    app.setOrganizationName("Imilya");
    app.setOrganizationDomain("imilya.minds");
    
    // Setup command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("A desktop search application with web scraping capabilities");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption searchOption(QStringList() << "s" << "search",
                                   "Perform search and exit", "query");
    parser.addOption(searchOption);
    
    QCommandLineOption engineOption(QStringList() << "e" << "engine", 
                                   "Search engine to use (google, bing, duckduckgo)", "engine", "google");
    parser.addOption(engineOption);
    
    QCommandLineOption noGuiOption(QStringList() << "no-gui",
                                  "Run without GUI (command line only)");
    parser.addOption(noGuiOption);
    
    parser.process(app);
    
    // Ensure data directory exists
    ensureDataDirectory();
    
    // Handle command line search
    if (parser.isSet(searchOption)) {
        QString query = parser.value(searchOption);
        QString engine = parser.value(engineOption);
        
        // TODO: Implement command line search
        qDebug() << "Command line search:" << query << "using" << engine;
        
        if (parser.isSet(noGuiOption)) {
            return 0; // Exit after search
        }
    }
    
    // Setup application style
    setupApplicationStyle();
    
    // Load custom fonts if available
    QString fontDir = QApplication::applicationDirPath() + "/resources/fonts/";
    if (QDir(fontDir).exists()) {
        QFontDatabase::addApplicationFont(fontDir + "OpenSans-Regular.ttf");
        QFontDatabase::addApplicationFont(fontDir + "OpenSans-Bold.ttf");
        app.setFont(QFont("Open Sans", 10));
    }
    
    // Create and show main window
    MainWindow window;
    
    // Center window on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - window.width()) / 2;
        int y = (screenGeometry.height() - window.height()) / 2;
        window.move(x, y);
    }
    
    window.show();
    
    // If search query was provided, perform it in the GUI
    if (parser.isSet(searchOption)) {
        // TODO: Trigger search in the main window
        QString query = parser.value(searchOption);
        // window.performSearch(query);
    }
    
    return app.exec();
}
