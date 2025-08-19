#include "OfflineQADatabase.h"
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDir>

OfflineQADatabase::OfflineQADatabase(QObject *parent)
    : QObject(parent)
{
    initializeDatabase();
}

OfflineQADatabase::~OfflineQADatabase()
{
}

void OfflineQADatabase::initializeDatabase()
{
    addGreetings();
    addCommonQuestions();
    addTechnicalQuestions();
    addGeneralKnowledge();
    loadExternalData();
    
    qDebug() << "Offline Q&A Database initialized with" << m_qaDatabase.size() << "entries";
}
void OfflineQADatabase::loadExternalData()
{
    QString baseDir = QApplication::applicationDirPath() + "/resources/data";
    QDir dir(baseDir);
    if (!dir.exists()) return;
    QStringList jsonFiles = dir.entryList({"*.json"}, QDir::Files);
    QStringList csvFiles = dir.entryList({"*.csv"}, QDir::Files);
    for (const QString& f : jsonFiles) addFromJsonFile(dir.filePath(f));
    for (const QString& f : csvFiles) addFromCsvFile(dir.filePath(f));
}

void OfflineQADatabase::addFromJsonFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) return;
    if (doc.isArray()) {
        QJsonArray arr = doc.array();
        for (const QJsonValue& v : arr) {
            if (!v.isObject()) continue;
            QJsonObject o = v.toObject();
            QString q = o.value("question").toString();
            QString a = o.value("answer").toString();
            QString c = o.value("category").toString("General");
            if (!q.trimmed().isEmpty() && !a.trimmed().isEmpty()) {
                addQA(q, a, c);
            }
        }
    } else if (doc.isObject()) {
        QJsonObject root = doc.object();
        QJsonArray arr = root.value("items").toArray();
        for (const QJsonValue& v : arr) {
            if (!v.isObject()) continue;
            QJsonObject o = v.toObject();
            QString q = o.value("question").toString();
            QString a = o.value("answer").toString();
            QString c = o.value("category").toString("General");
            if (!q.trimmed().isEmpty() && !a.trimmed().isEmpty()) {
                addQA(q, a, c);
            }
        }
    }
}

void OfflineQADatabase::addFromCsvFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    // Simple CSV: question,answer,category
    while (!in.atEnd()) {
        const QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;
        const QStringList parts = line.split(",");
        if (parts.size() < 2) continue;
        QString q = parts.at(0).trimmed();
        QString a = parts.at(1).trimmed();
        QString c = parts.size() >= 3 ? parts.at(2).trimmed() : QString("General");
        if (!q.isEmpty() && !a.isEmpty()) addQA(q, a, c);
    }
    file.close();
}

void OfflineQADatabase::addGreetings()
{
    // Basic greetings
    addQA("hello", "Hello! How can I help you today? I'm your personal search assistant.", "Greetings");
    addQA("hi", "Hi there! Welcome to your search app. What would you like to know?", "Greetings");
    addQA("hey", "Hey! I'm here to help you find information. What's on your mind?", "Greetings");
    addQA("good morning", "Good morning! I hope you're having a great start to your day. How can I assist you?", "Greetings");
    addQA("good afternoon", "Good afternoon! I'm ready to help you with your search queries.", "Greetings");
    addQA("good evening", "Good evening! I'm here to help you find what you're looking for.", "Greetings");
    addQA("good night", "Good night! I hope you had a productive day. Feel free to ask if you need anything.", "Greetings");
    addQA("morning", "Good morning! How can I help you today?", "Greetings");
    addQA("afternoon", "Good afternoon! What would you like to search for?", "Greetings");
    addQA("evening", "Good evening! I'm here to help you.", "Greetings");
    addQA("night", "Good night! I hope you had a great day.", "Greetings");
    
    // Informal greetings
    addQA("sup", "Sup! What's up with you? Need help finding something?", "Greetings");
    addQA("what's up", "Not much, just here to help you search! What do you need?", "Greetings");
    addQA("howdy", "Howdy! I'm your search partner. What can I help you find?", "Greetings");
    addQA("yo", "Yo! Ready to help you search. What's the plan?", "Greetings");
    
    // Polite greetings
    addQA("greetings", "Greetings! I'm honored to assist you today. What would you like to know?", "Greetings");
    addQA("salutations", "Salutations! I'm here to help you find information. How may I assist?", "Greetings");
    addQA("good day", "Good day to you! I'm ready to help with your search needs.", "Greetings");
}

void OfflineQADatabase::addCommonQuestions()
{
    // Personal questions
    addQA("how are you", "I'm doing great, thank you for asking! I'm a search assistant, so I'm always ready to help you find information. How about you?", "Personal");
    addQA("what's your name", "My name is Search Assistant! I'm here to help you find information quickly and efficiently.", "Personal");
    addQA("who are you", "I'm your personal search assistant, designed to help you find information both offline and online. I can answer common questions instantly and search the web when needed.", "Personal");
    addQA("what can you do", "I can do many things! I can answer common questions instantly, provide search suggestions, search multiple engines simultaneously, and even open results in your browser. I'm designed to be fast, responsive, and helpful.", "Personal");
    
    // Help requests
    addQA("help", "I'm here to help! I can:\n• Answer common questions instantly\n• Search the web for you\n• Provide search suggestions\n• Open results in your browser\n\nJust type your question or search term and I'll assist you!", "Help");
    addQA("help me", "I'd be happy to help you! What do you need assistance with? I can answer questions, search the web, or provide guidance on how to use this app.", "Help");
    addQA("i need help", "No worries, I'm here to help! What can I assist you with today? Feel free to ask any question or search for any topic.", "Help");
    addQA("can you help", "Absolutely! I'm designed to help you find information quickly and easily. What do you need help with?", "Help");
    
    // App usage
    addQA("how to use", "Using this app is simple:\n\n1. Type your question or search term in the search box\n2. Press Enter or click the search button\n3. I'll show you instant answers if available\n4. If no instant answer, I'll search the web for you\n5. Click any result to open it in your browser\n\nTry asking me something!", "Usage");
    addQA("how does this work", "This app works by combining offline knowledge with web search:\n\n• First, I check if I have an instant answer for your question\n• If not, I search multiple search engines simultaneously\n• I show you the best results with relevance scores\n• You can click any result to open it in your browser\n\nIt's designed to be fast and comprehensive!", "Usage");
    addQA("what is this app", "This is a smart search application that combines:\n\n• Instant offline answers for common questions\n• Multi-engine web search capabilities\n• Intelligent result ranking and filtering\n• Beautiful, responsive user interface\n• Fast search suggestions\n\nIt's designed to give you the best of both worlds - instant answers when possible, and comprehensive web search when needed.", "Usage");
}

void OfflineQADatabase::addTechnicalQuestions()
{
    // Technology
    addQA("what is ai", "AI (Artificial Intelligence) is technology that enables computers to perform tasks that typically require human intelligence, such as learning, reasoning, problem-solving, and understanding natural language.", "Technology");
    addQA("what is machine learning", "Machine Learning is a subset of AI that allows computers to learn and improve from experience without being explicitly programmed. It uses algorithms to identify patterns in data.", "Technology");
    addQA("what is programming", "Programming is the process of creating instructions for computers to follow. It involves writing code in programming languages to solve problems and create software applications.", "Technology");
    addQA("what is coding", "Coding is the act of writing computer programs using programming languages. It's how we communicate with computers to make them perform specific tasks.", "Technology");
    
    // Internet and Web
    addQA("what is the internet", "The Internet is a global network of connected computers that allows people to share information, communicate, and access resources from anywhere in the world.", "Technology");
    addQA("what is a website", "A website is a collection of web pages hosted on the internet that can contain text, images, videos, and other content accessible through a web browser.", "Technology");
    addQA("what is a browser", "A web browser is software that allows you to access and view websites on the internet. Examples include Chrome, Firefox, Safari, and Edge.", "Technology");
    
    // Software
    addQA("what is software", "Software is a set of instructions and data that tell a computer how to perform specific tasks. It includes applications, operating systems, and utilities.", "Technology");
    addQA("what is an app", "An app (application) is software designed to perform specific functions for users. Apps can run on computers, smartphones, tablets, and other devices.", "Technology");
    addQA("what is an operating system", "An operating system (OS) is software that manages computer hardware and software resources, providing common services for computer programs.", "Technology");
    // More technology Q&A
    addQA("what is http", "HTTP (Hypertext Transfer Protocol) is an application protocol used for transmitting hypermedia documents, such as HTML.", "Technology");
    addQA("what is https", "HTTPS is HTTP over TLS/SSL, providing encryption and authentication for secure communication over networks.", "Technology");
    addQA("what is api", "An API (Application Programming Interface) is a set of rules that allow different software entities to communicate.", "Technology");
    addQA("what is rest", "REST (Representational State Transfer) is an architectural style for designing networked applications using stateless requests.", "Technology");
    addQA("what is graphql", "GraphQL is a query language for APIs that lets clients request exactly the data they need.", "Technology");
    addQA("what is database", "A database is an organized collection of structured information, typically stored electronically.", "Technology");
    addQA("what is sql", "SQL (Structured Query Language) is used to manage and query data in relational databases.", "Technology");
    addQA("what is nosql", "NoSQL databases provide flexible schemas and scale horizontally, suitable for large distributed data.", "Technology");
    addQA("what is cloud computing", "Cloud computing is the delivery of computing services over the internet on-demand and pay-as-you-go.", "Technology");
    addQA("what is docker", "Docker is a platform to build, ship, and run applications inside lightweight containers.", "Technology");
    addQA("what is kubernetes", "Kubernetes is an open-source system for automating deployment, scaling, and management of containerized applications.", "Technology");
    addQA("what is version control", "Version control tracks changes to files over time so you can recall specific versions later.", "Technology");
    addQA("what is git", "Git is a distributed version control system for tracking changes in source code.", "Technology");
    addQA("what is github", "GitHub is a platform for hosting Git repositories with collaboration features like pull requests and issues.", "Technology");
    addQA("what is python", "Python is a high-level, interpreted programming language known for readability and rich ecosystem.", "Programming");
    addQA("what is javascript", "JavaScript is a versatile language primarily used to create interactive behavior on web pages.", "Programming");
    addQA("what is c++", "C++ is a general-purpose programming language with object-oriented and generic programming features.", "Programming");
    addQA("what is java", "Java is a class-based, object-oriented programming language designed to have as few implementation dependencies as possible.", "Programming");
    addQA("what is html", "HTML (HyperText Markup Language) structures content on the web.", "Web");
    addQA("what is css", "CSS (Cascading Style Sheets) describes the presentation of HTML documents.", "Web");
}

void OfflineQADatabase::addGeneralKnowledge()
{
    // Science
    addQA("what is gravity", "Gravity is a fundamental force that attracts objects toward each other. On Earth, it pulls everything toward the center of the planet, which is why objects fall when dropped.", "Science");
    addQA("what is photosynthesis", "Photosynthesis is the process by which plants convert sunlight, carbon dioxide, and water into glucose and oxygen. It's how plants make their own food.", "Science");
    addQA("what is dna", "DNA (Deoxyribonucleic acid) is a molecule that carries genetic information and instructions for the development and functioning of living organisms.", "Science");
    
    // Geography
    addQA("what is the capital of france", "The capital of France is Paris, known as the 'City of Light' and famous for its culture, art, fashion, and landmarks like the Eiffel Tower.", "Geography");
    addQA("what is the largest ocean", "The Pacific Ocean is the largest ocean on Earth, covering about 46% of the Earth's water surface and about one-third of its total surface area.", "Geography");
    addQA("what is the highest mountain", "Mount Everest is the highest mountain above sea level, with a peak elevation of 29,029 feet (8,848 meters) above sea level.", "Geography");
    
    // History
    addQA("who invented the telephone", "Alexander Graham Bell is credited with inventing the first practical telephone in 1876, though there were earlier developments by other inventors.", "History");
    addQA("when was world war 2", "World War II lasted from 1939 to 1945, involving most of the world's nations and resulting in significant global changes.", "History");
    addQA("who was albert einstein", "Albert Einstein was a German-born theoretical physicist who developed the theory of relativity, one of the two pillars of modern physics. He won the Nobel Prize in Physics in 1921.", "History");
    
    // Math
    addQA("what is pi", "Pi (π) is a mathematical constant representing the ratio of a circle's circumference to its diameter. Its approximate value is 3.14159, though it's an irrational number with infinite decimal places.", "Math");
    addQA("what is the square root", "The square root of a number is a value that, when multiplied by itself, gives the original number. For example, the square root of 16 is 4, because 4 × 4 = 16.", "Math");
    addQA("what is multiplication", "Multiplication is a mathematical operation that combines groups of equal size. It's essentially repeated addition. For example, 3 × 4 means adding 3 four times: 3 + 3 + 3 + 3 = 12.", "Math");
    
    // Language
    addQA("what is a noun", "A noun is a word that names a person, place, thing, or idea. Examples include: person (John), place (Paris), thing (book), idea (freedom).", "Language");
    addQA("what is a verb", "A verb is a word that expresses an action, occurrence, or state of being. Examples include: run, jump, think, be, have.", "Language");
    addQA("what is an adjective", "An adjective is a word that describes or modifies a noun or pronoun. Examples include: big, red, happy, beautiful, intelligent.", "Language");
    
    // Health
    addQA("what is exercise", "Exercise is physical activity that improves health, fitness, and overall well-being. It includes activities like walking, running, swimming, and strength training.", "Health");
    addQA("what is nutrition", "Nutrition is the science of how food affects the body. It involves understanding the nutrients in food and how they contribute to health and disease prevention.", "Health");
    addQA("what is sleep", "Sleep is a natural state of rest for the mind and body, essential for physical and mental health, memory consolidation, and overall well-being.", "Health");
    
    // Entertainment
    addQA("what is music", "Music is an art form that uses sound and silence organized in time. It can include melody, harmony, rhythm, and timbre to create expressive and meaningful compositions.", "Entertainment");
    addQA("what is a movie", "A movie (or film) is a series of moving images shown on a screen, typically with accompanying sound, that tells a story or presents information.", "Entertainment");
    addQA("what is art", "Art is the expression or application of human creative skill and imagination, typically in visual form such as painting, sculpture, or other creative works.", "Entertainment");
    
    // Business
    addQA("what is entrepreneurship", "Entrepreneurship is the process of starting and running a business, taking on financial risks in the hope of profit. Entrepreneurs identify opportunities and create value.", "Business");
    addQA("what is marketing", "Marketing is the process of promoting, selling, and distributing products or services. It involves understanding customer needs and creating strategies to meet them.", "Business");
    addQA("what is innovation", "Innovation is the process of creating new ideas, methods, or products that provide value. It often involves improving existing solutions or creating entirely new ones.", "Business");
    
    // Geography - more capitals
    addQA("what is the capital of japan", "Tokyo is the capital of Japan, a bustling metropolis known for its technology and culture.", "Geography");
    addQA("what is the capital of india", "New Delhi is the capital of India, serving as the seat of all three branches of the Government of India.", "Geography");
    addQA("what is the capital of canada", "Ottawa is the capital of Canada, located in the province of Ontario.", "Geography");
    
    // Math - more
    addQA("what is algebra", "Algebra is a branch of mathematics dealing with symbols and the rules for manipulating those symbols.", "Math");
    addQA("what is calculus", "Calculus is the mathematical study of continuous change, dealing with derivatives and integrals.", "Math");
}

void OfflineQADatabase::addQA(const QString& question, const QString& answer, const QString& category)
{
    SearchResult result;
    result.title = question;
    result.description = answer;
    result.url = "offline://" + category.toLower();
    result.displayUrl = "Offline Answer - " + category;
    result.sourceEngine = "Offline Database";
    result.relevanceScore = 1.0;
    result.timestamp = QDateTime::currentDateTime();
    
    // Store with multiple variations of the question
    QString cleanQuestion = question.toLower().trimmed();
    m_qaDatabase[cleanQuestion] = result;
    m_allQuestions.append(question);
    
    // Add variations
    QStringList variations;
    variations << question.toLower() << question.toUpper() << question.trimmed();
    
    for (const QString& variation : variations) {
        if (!m_qaDatabase.contains(variation)) {
            m_qaDatabase[variation] = result;
        }
    }
}

bool OfflineQADatabase::hasOfflineAnswer(const QString& query) const
{
    QString cleanQuery = query.toLower().trimmed();
    
    // Direct match
    if (m_qaDatabase.contains(cleanQuery)) {
        return true;
    }
    
    // Partial match
    for (const QString& question : m_allQuestions) {
        if (question.toLower().contains(cleanQuery) || cleanQuery.contains(question.toLower())) {
            return true;
        }
    }
    
    return false;
}

SearchResult OfflineQADatabase::getOfflineAnswer(const QString& query) const
{
    QString cleanQuery = query.toLower().trimmed();
    
    // Direct match
    if (m_qaDatabase.contains(cleanQuery)) {
        return m_qaDatabase[cleanQuery];
    }
    
    // Partial match - find the best match
    SearchResult bestMatch;
    double bestScore = 0.0;
    
    for (const QString& question : m_allQuestions) {
        QString lowerQuestion = question.toLower();
        
        // Check if query contains question or question contains query
        if (lowerQuestion.contains(cleanQuery) || cleanQuery.contains(lowerQuestion)) {
            double score = 0.0;
            
            // Exact match gets highest score
            if (lowerQuestion == cleanQuery) {
                score = 1.0;
            }
            // Query contains question
            else if (lowerQuestion.contains(cleanQuery)) {
                score = 0.8;
            }
            // Question contains query
            else if (cleanQuery.contains(lowerQuestion)) {
                score = 0.6;
            }
            
            if (score > bestScore) {
                bestScore = score;
                bestMatch = m_qaDatabase[question.toLower()];
            }
        }
    }
    
    return bestMatch;
}

QVector<SearchResult> OfflineQADatabase::getAllOfflineAnswers() const
{
    QVector<SearchResult> results;
    for (const SearchResult& result : m_qaDatabase.values()) {
        results.append(result);
    }
    return results;
}

QStringList OfflineQADatabase::getSuggestions(const QString& partialQuery) const
{
    QStringList suggestions;
    QString lowerQuery = partialQuery.toLower().trimmed();
    
    if (lowerQuery.isEmpty()) {
        return suggestions;
    }
    
    for (const QString& question : m_allQuestions) {
        if (question.toLower().contains(lowerQuery)) {
            suggestions.append(question);
            if (suggestions.size() >= 10) { // Limit suggestions
                break;
            }
        }
    }
    
    return suggestions;
}
