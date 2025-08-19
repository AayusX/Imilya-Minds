#pragma once

#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QPixmap>

/**
 * Represents a single search result
 */
struct SearchResult {
    QString title;
    QString description;
    QUrl url;
    QString displayUrl;
    QDateTime timestamp;
    QPixmap favicon;
    
    // Relevance score (0.0 to 1.0)
    double relevanceScore = 0.0;
    
    // Source engine that found this result
    QString sourceEngine;
    
    SearchResult() = default;
    
    SearchResult(const QString& title, const QString& description, const QUrl& url)
        : title(title), description(description), url(url), timestamp(QDateTime::currentDateTime()) {
        displayUrl = url.toString();
    }
    
    bool isValid() const {
        return !title.isEmpty() && url.isValid();
    }
    
    // For sorting by relevance
    bool operator<(const SearchResult& other) const {
        return relevanceScore > other.relevanceScore; // Higher score first
    }
};
