#include "ResultsWidget.h"
#include <QListWidgetItem>

ResultsWidget::ResultsWidget(QWidget *parent) : QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    
    m_statusLabel = new QLabel("No results", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet(R"(
        QLabel { 
            color: #888888; 
            font-style: italic; 
            font-size: 16px;
            padding: 24px;
            background-color: rgba(255,255,255,0.04);
            border: 1px dashed rgba(255,255,255,0.1);
            border-radius: 12px;
            margin: 12px;
        }
    )");
    
    m_resultsList = new QListWidget(this);
    m_resultsList->setStyleSheet(R"(
        QListWidget {
            background-color: transparent;
            border: none;
            outline: none;
            font-size: 14px;
        }
        QListWidget::item {
            background-color: rgba(255,255,255,0.04);
            border: 1px solid rgba(255,255,255,0.08);
            border-radius: 12px;
            margin: 6px;
            padding: 14px;
            color: #ffffff;
        }
        QListWidget::item:hover {
            background-color: rgba(0,212,255,0.10);
            border-color: #00d4ff;
        }
        QListWidget::item:selected {
            background-color: #00d4ff;
            color: #000000;
            border-color: #00d4ff;
        }
    )");
    
    connect(m_resultsList, &QListWidget::itemClicked, this, &ResultsWidget::onItemClicked);
    
    m_layout->addWidget(m_statusLabel);
    m_layout->addWidget(m_resultsList);
    
    setLayout(m_layout);
}

void ResultsWidget::displayResults(const QVector<SearchResult>& results) {
    m_results = results;
    m_resultsList->clear();
    
    if (results.isEmpty()) {
        m_statusLabel->setText("No results found");
        m_statusLabel->show();
        m_resultsList->hide();
        return;
    }
    
    m_statusLabel->hide();
    m_resultsList->show();
    
    for (const SearchResult& result : results) {
        QListWidgetItem* item = new QListWidgetItem();
        
        QString relevanceIcon = "⭐";
        if (result.relevanceScore >= 0.8) relevanceIcon = "🔥";
        else if (result.relevanceScore >= 0.6) relevanceIcon = "⭐";
        else if (result.relevanceScore >= 0.4) relevanceIcon = "⚡";
        else relevanceIcon = "💡";
        
        QString itemText = QString(
            "<div style='margin: 4px;'>"
            "<div style='color: #00d4ff; font-weight: 700; font-size: 15px;'>%1 %2</div>"
            "<div style='color: #9aa4af; font-size: 12px; margin: 2px 0;'>%3</div>"
            "<div style='color: #e0e3e6; font-size: 13px; line-height: 1.4;'>%4</div>"
            "<div style='color: #00d4ff; font-size: 11px; margin-top: 6px;'>"
            "Source: %5 • Relevance: %6% • %7"
            "</div>"
            "</div>"
        ).arg(
            relevanceIcon,
            result.title,
            result.displayUrl,
            result.description,
            result.sourceEngine,
            QString::number(int(result.relevanceScore * 100)),
            result.timestamp.toString("MMM dd, hh:mm")
        );
        
        item->setText(itemText);
        item->setData(Qt::UserRole, QVariant::fromValue(result));
        item->setSizeHint(QSize(0, 110));
        
        m_resultsList->addItem(item);
    }
    
    m_statusLabel->setText(QString("Found %1 results").arg(results.size()));
}

void ResultsWidget::clearResults() {
    m_results.clear();
    m_resultsList->clear();
    m_statusLabel->setText("No results");
    m_resultsList->show();
    m_statusLabel->show();
}

void ResultsWidget::onItemClicked() {
    QListWidgetItem* item = m_resultsList->currentItem();
    if (item) {
        SearchResult result = item->data(Qt::UserRole).value<SearchResult>();
        emit resultClicked(result);
    }
}

void ResultsWidget::applyLightStyle() {
    m_resultsList->setStyleSheet(R"(
        QListWidget { background-color: transparent; border: none; outline: none; font-size: 14px; }
        QListWidget::item { background-color: #ffffff; border: 1px solid #e1e4ea; border-radius: 12px; margin: 6px; padding: 14px; color: #111111; }
        QListWidget::item:hover { background-color: #f0f5ff; border-color: #4a90e2; }
        QListWidget::item:selected { background-color: #4a90e2; color: #ffffff; border-color: #4a90e2; }
    )");
}

void ResultsWidget::applyDarkStyle() {
    m_resultsList->setStyleSheet(R"(
        QListWidget { background-color: transparent; border: none; outline: none; font-size: 14px; }
        QListWidget::item { background-color: rgba(255,255,255,0.04); border: 1px solid rgba(255,255,255,0.08); border-radius: 12px; margin: 6px; padding: 14px; color: #ffffff; }
        QListWidget::item:hover { background-color: rgba(0,212,255,0.10); border-color: #00d4ff; }
        QListWidget::item:selected { background-color: #00d4ff; color: #000000; border-color: #00d4ff; }
    )");
}

#include "ResultsWidget.moc"
