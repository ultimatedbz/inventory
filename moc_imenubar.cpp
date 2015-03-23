/****************************************************************************
** Meta object code from reading C++ file 'imenubar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "imenubar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imenubar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IMenuBar_t {
    QByteArrayData data[20];
    char stringdata[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IMenuBar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IMenuBar_t qt_meta_stringdata_IMenuBar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "IMenuBar"
QT_MOC_LITERAL(1, 9, 7), // "newFile"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "loadFile"
QT_MOC_LITERAL(4, 27, 4), // "save"
QT_MOC_LITERAL(5, 32, 6), // "saveAs"
QT_MOC_LITERAL(6, 39, 14), // "printInventory"
QT_MOC_LITERAL(7, 54, 12), // "printHistory"
QT_MOC_LITERAL(8, 67, 17), // "printTransactions"
QT_MOC_LITERAL(9, 85, 15), // "changeToEnglish"
QT_MOC_LITERAL(10, 101, 15), // "changeToChinese"
QT_MOC_LITERAL(11, 117, 15), // "showPreferences"
QT_MOC_LITERAL(12, 133, 12), // "addVegetable"
QT_MOC_LITERAL(13, 146, 10), // "addCompany"
QT_MOC_LITERAL(14, 157, 9), // "addPerson"
QT_MOC_LITERAL(15, 167, 7), // "addUnit"
QT_MOC_LITERAL(16, 175, 13), // "removeCompany"
QT_MOC_LITERAL(17, 189, 12), // "removePerson"
QT_MOC_LITERAL(18, 202, 15), // "removeVegetable"
QT_MOC_LITERAL(19, 218, 10) // "removeUnit"

    },
    "IMenuBar\0newFile\0\0loadFile\0save\0saveAs\0"
    "printInventory\0printHistory\0"
    "printTransactions\0changeToEnglish\0"
    "changeToChinese\0showPreferences\0"
    "addVegetable\0addCompany\0addPerson\0"
    "addUnit\0removeCompany\0removePerson\0"
    "removeVegetable\0removeUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IMenuBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x0a /* Public */,
       3,    0,  105,    2, 0x0a /* Public */,
       4,    0,  106,    2, 0x0a /* Public */,
       5,    0,  107,    2, 0x0a /* Public */,
       6,    0,  108,    2, 0x0a /* Public */,
       7,    0,  109,    2, 0x0a /* Public */,
       8,    0,  110,    2, 0x0a /* Public */,
       9,    0,  111,    2, 0x0a /* Public */,
      10,    0,  112,    2, 0x0a /* Public */,
      11,    0,  113,    2, 0x0a /* Public */,
      12,    0,  114,    2, 0x0a /* Public */,
      13,    0,  115,    2, 0x0a /* Public */,
      14,    0,  116,    2, 0x0a /* Public */,
      15,    0,  117,    2, 0x0a /* Public */,
      16,    0,  118,    2, 0x0a /* Public */,
      17,    0,  119,    2, 0x0a /* Public */,
      18,    0,  120,    2, 0x0a /* Public */,
      19,    0,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IMenuBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IMenuBar *_t = static_cast<IMenuBar *>(_o);
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->loadFile(); break;
        case 2: _t->save(); break;
        case 3: _t->saveAs(); break;
        case 4: _t->printInventory(); break;
        case 5: _t->printHistory(); break;
        case 6: _t->printTransactions(); break;
        case 7: _t->changeToEnglish(); break;
        case 8: _t->changeToChinese(); break;
        case 9: _t->showPreferences(); break;
        case 10: _t->addVegetable(); break;
        case 11: _t->addCompany(); break;
        case 12: _t->addPerson(); break;
        case 13: _t->addUnit(); break;
        case 14: _t->removeCompany(); break;
        case 15: _t->removePerson(); break;
        case 16: _t->removeVegetable(); break;
        case 17: _t->removeUnit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IMenuBar::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_IMenuBar.data,
      qt_meta_data_IMenuBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IMenuBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IMenuBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IMenuBar.stringdata))
        return static_cast<void*>(const_cast< IMenuBar*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int IMenuBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
