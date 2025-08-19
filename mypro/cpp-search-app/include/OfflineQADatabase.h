#ifndef OFFLINEQADATABASE_H
#define OFFLINEQADATABASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QVector>
#include "SearchResult.h"

class OfflineQADatabase : public QObject
{
    Q_OBJECT

public:
    explicit OfflineQADatabase(QObject *parent = nullptr);
    ~OfflineQADatabase();

    // Check if query has an offline answer
    bool hasOfflineAnswer(const QString& query) const;
    
    // Get offline answer for a query
    SearchResult getOfflineAnswer(const QString& query) const;
    
    // Get all available offline answers
    QVector<SearchResult> getAllOfflineAnswers() const;
    
    // Get suggestions based on partial query
    QStringList getSuggestions(const QString& partialQuery) const;

private:
    void initializeDatabase();
    void addQA(const QString& question, const QString& answer, const QString& category = "General");
    void addGreetings();
    void addCommonQuestions();
    void addTechnicalQuestions();
    void addGeneralKnowledge();
    void loadExternalData();
    void addFromJsonFile(const QString& filePath);
    void addFromCsvFile(const QString& filePath);
    
    QHash<QString, SearchResult> m_qaDatabase;
    QStringList m_allQuestions;
};

#endif // OFFLINEQADABASE_H
