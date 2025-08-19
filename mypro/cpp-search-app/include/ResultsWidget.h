#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "SearchResult.h"

/**
 * Widget to display search results
 */
class ResultsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    
    void displayResults(const QVector<SearchResult>& results);
    void clearResults();

    // Theme-aware styling
    void applyLightStyle();
    void applyDarkStyle();

signals:
    void resultClicked(const SearchResult& result);

private slots:
    void onItemClicked();

private:
    QVBoxLayout* m_layout;
    QLabel* m_statusLabel;
    QListWidget* m_resultsList;
    QVector<SearchResult> m_results;
};

