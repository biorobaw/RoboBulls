/****************************************************************************
** Meta object code from reading C++ file 'fieldpanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui/fieldpanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fieldpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FieldPanel_t {
    QByteArrayData data[17];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FieldPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FieldPanel_t qt_meta_stringdata_FieldPanel = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FieldPanel"
QT_MOC_LITERAL(1, 11, 9), // "zoomField"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 5), // "scale"
QT_MOC_LITERAL(4, 28, 11), // "defaultZoom"
QT_MOC_LITERAL(5, 40, 8), // "drawLine"
QT_MOC_LITERAL(6, 49, 9), // "drawPoint"
QT_MOC_LITERAL(7, 59, 10), // "drawRegion"
QT_MOC_LITERAL(8, 70, 9), // "setupLine"
QT_MOC_LITERAL(9, 80, 5), // "Point"
QT_MOC_LITERAL(10, 86, 5), // "start"
QT_MOC_LITERAL(11, 92, 4), // "stop"
QT_MOC_LITERAL(12, 97, 7), // "seconds"
QT_MOC_LITERAL(13, 105, 10), // "setupPoint"
QT_MOC_LITERAL(14, 116, 1), // "p"
QT_MOC_LITERAL(15, 118, 11), // "setupRegion"
QT_MOC_LITERAL(16, 130, 18) // "std::vector<Point>"

    },
    "FieldPanel\0zoomField\0\0scale\0defaultZoom\0"
    "drawLine\0drawPoint\0drawRegion\0setupLine\0"
    "Point\0start\0stop\0seconds\0setupPoint\0"
    "p\0setupRegion\0std::vector<Point>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FieldPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    0,   57,    2, 0x0a /* Public */,
       5,    0,   58,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    3,   61,    2, 0x0a /* Public */,
      13,    1,   68,    2, 0x0a /* Public */,
      15,    1,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9, QMetaType::Double,   10,   11,   12,
    QMetaType::Void, 0x80000000 | 9,   14,
    QMetaType::Void, 0x80000000 | 16,    2,

       0        // eod
};

void FieldPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FieldPanel *_t = static_cast<FieldPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomField((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->defaultZoom(); break;
        case 2: _t->drawLine(); break;
        case 3: _t->drawPoint(); break;
        case 4: _t->drawRegion(); break;
        case 5: _t->setupLine((*reinterpret_cast< Point(*)>(_a[1])),(*reinterpret_cast< Point(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 6: _t->setupPoint((*reinterpret_cast< Point(*)>(_a[1]))); break;
        case 7: _t->setupRegion((*reinterpret_cast< std::vector<Point>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FieldPanel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FieldPanel.data,
      qt_meta_data_FieldPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FieldPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FieldPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FieldPanel.stringdata0))
        return static_cast<void*>(const_cast< FieldPanel*>(this));
    return QObject::qt_metacast(_clname);
}

int FieldPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
