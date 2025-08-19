/****************************************************************************
** Meta object code from reading C++ file 'SearchEngine.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/SearchEngine.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_SearchEngine_t {
    uint offsetsAndSizes[32];
    char stringdata0[13];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[8];
    char stringdata5[12];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[11];
    char stringdata9[17];
    char stringdata10[12];
    char stringdata11[22];
    char stringdata12[26];
    char stringdata13[16];
    char stringdata14[23];
    char stringdata15[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SearchEngine_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SearchEngine_t qt_meta_stringdata_SearchEngine = {
    {
        QT_MOC_LITERAL(0, 12),  // "SearchEngine"
        QT_MOC_LITERAL(13, 14),  // "searchFinished"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 19),  // "QList<SearchResult>"
        QT_MOC_LITERAL(49, 7),  // "results"
        QT_MOC_LITERAL(57, 11),  // "searchError"
        QT_MOC_LITERAL(69, 5),  // "error"
        QT_MOC_LITERAL(75, 14),  // "searchProgress"
        QT_MOC_LITERAL(90, 10),  // "percentage"
        QT_MOC_LITERAL(101, 16),  // "suggestionsReady"
        QT_MOC_LITERAL(118, 11),  // "suggestions"
        QT_MOC_LITERAL(130, 21),  // "onSearchReplyFinished"
        QT_MOC_LITERAL(152, 25),  // "onSuggestionReplyFinished"
        QT_MOC_LITERAL(178, 15),  // "onSearchTimeout"
        QT_MOC_LITERAL(194, 22),  // "onEngineSearchFinished"
        QT_MOC_LITERAL(217, 6)   // "engine"
    },
    "SearchEngine",
    "searchFinished",
    "",
    "QList<SearchResult>",
    "results",
    "searchError",
    "error",
    "searchProgress",
    "percentage",
    "suggestionsReady",
    "suggestions",
    "onSearchReplyFinished",
    "onSuggestionReplyFinished",
    "onSearchTimeout",
    "onEngineSearchFinished",
    "engine"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SearchEngine[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       5,    1,   65,    2, 0x06,    3 /* Public */,
       7,    1,   68,    2, 0x06,    5 /* Public */,
       9,    1,   71,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    0,   74,    2, 0x08,    9 /* Private */,
      12,    0,   75,    2, 0x08,   10 /* Private */,
      13,    0,   76,    2, 0x08,   11 /* Private */,
      14,    2,   77,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QStringList,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject SearchEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SearchEngine.offsetsAndSizes,
    qt_meta_data_SearchEngine,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SearchEngine_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SearchEngine, std::true_type>,
        // method 'searchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<SearchResult> &, std::false_type>,
        // method 'searchError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'searchProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'suggestionsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'onSearchReplyFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSuggestionReplyFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEngineSearchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<SearchResult> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void SearchEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SearchEngine *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->searchFinished((*reinterpret_cast< std::add_pointer_t<QList<SearchResult>>>(_a[1]))); break;
        case 1: _t->searchError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->searchProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->suggestionsReady((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 4: _t->onSearchReplyFinished(); break;
        case 5: _t->onSuggestionReplyFinished(); break;
        case 6: _t->onSearchTimeout(); break;
        case 7: _t->onEngineSearchFinished((*reinterpret_cast< std::add_pointer_t<QList<SearchResult>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SearchEngine::*)(const QVector<SearchResult> & );
            if (_t _q_method = &SearchEngine::searchFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SearchEngine::*)(const QString & );
            if (_t _q_method = &SearchEngine::searchError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SearchEngine::*)(int );
            if (_t _q_method = &SearchEngine::searchProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SearchEngine::*)(const QStringList & );
            if (_t _q_method = &SearchEngine::suggestionsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *SearchEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SearchEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SearchEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SearchEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SearchEngine::searchFinished(const QVector<SearchResult> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SearchEngine::searchError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SearchEngine::searchProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SearchEngine::suggestionsReady(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
