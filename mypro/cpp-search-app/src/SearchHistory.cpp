#include "SearchHistory.h"

SearchHistory::SearchHistory(QObject *parent) 
    : QObject(parent), m_maxSize(100) {
    m_settings = new QSettings(this);
    loadHistory();
}

void SearchHistory::addSearch(const QString& query) {
    if (query.trimmed().isEmpty()) return;
    
    QString trimmed = query.trimmed();
    
    // Remove existing occurrence
    m_history.removeAll(trimmed);
    
    // Add to front
    m_history.prepend(trimmed);
    
    // Limit size
    if (m_history.size() > m_maxSize) {
        m_history = m_history.mid(0, m_maxSize);
    }
    
    saveHistory();
}

QStringList SearchHistory::getRecentSearches(int count) const {
    return m_history.mid(0, qMin(count, m_history.size()));
}

void SearchHistory::clearHistory() {
    m_history.clear();
    saveHistory();
}

QStringList SearchHistory::getSuggestions(const QString& prefix) const {
    QStringList suggestions;
    QString lowerPrefix = prefix.toLower();
    
    for (const QString& query : m_history) {
        if (query.toLower().startsWith(lowerPrefix) && query != prefix) {
            suggestions.append(query);
            if (suggestions.size() >= 10) break;
        }
    }
    
    return suggestions;
}

void SearchHistory::loadHistory() {
    m_history = m_settings->value("search_history", QStringList()).toStringList();
}

void SearchHistory::saveHistory() {
    m_settings->setValue("search_history", m_history);
    m_settings->sync();
}

#include "SearchHistory.moc"
