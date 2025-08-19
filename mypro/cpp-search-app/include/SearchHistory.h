#pragma once

#include <QObject>
#include <QStringList>
#include <QSettings>

/**
 * Search history manager
 */
class SearchHistory : public QObject {
    Q_OBJECT

public:
    explicit SearchHistory(QObject *parent = nullptr);
    
    void addSearch(const QString& query);
    QStringList getRecentSearches(int count = 10) const;
    void clearHistory();
    QStringList getSuggestions(const QString& prefix) const;

private:
    void loadHistory();
    void saveHistory();
    
    QStringList m_history;
    int m_maxSize;
    QSettings* m_settings;
};
