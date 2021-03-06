#ifndef CONTACTTABLEMODEL_H
#define CONTACTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

#include "contactdatamanager.h"

class ContactTablePriv;
class CWallet;
class WalletModel;

/**
   Qt model of the address book in the core. This allows views to access and modify the address book.
 */
class ContactTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ContactTableModel(ContactDataManager ccdb, WalletModel *parent = 0);
    ~ContactTableModel();

    enum ColumnIndex {
        Label = 0,
        Address = 1,
        Email = 2,
        URL = 3
    };

    enum RoleIndex {
        TypeRole = Qt::UserRole /**< Type of address (#Send or #Receive) */
    };

    /** Return status of edit/insert operation */
    enum EditStatus {
        OK,
        INVALID_ADDRESS,   /**< Unparseable address */
        DUPLICATE_ADDRESS,  /**< Address already in address book */
        WALLET_UNLOCK_FAILURE, /**< Wallet could not be unlocked to create new receiving address */
        KEY_GENERATION_FAILURE /**< Generating a new public key for a receiving address failed */
    };

    /** @name Methods overridden from QAbstractTableModel
        @{*/
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex & parent) const;
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex & index) const;
    /*@}*/

    /* Add an address to the model.
       Returns the added address on success, and an empty string otherwise.
     */
    QString addRow(const QString &label, const QString &address, const QString &email, const QString &url);

    /* Look up label for address in address book, if not found return empty string.
     */
    QString labelForAddress(const QString &address) const;

    /* Look up row index of an address in the model.
       Return -1 if not found.
     */
    int lookupAddress(const QString &address) const;

    EditStatus getEditStatus() const { return editStatus; }

    ContactDataManager contactDataBase(void);

private:
    WalletModel *walletModel;
    ContactDataManager ccdb;
    ContactTablePriv *priv;
    QStringList columns;
    EditStatus editStatus;

    /** Notify listeners that data changed. */
    void emitDataChanged(int index);

signals:
    void defaultAddressChanged(const QString &address);

public slots:
    /* Update address list from core.
     */
    void updateEntry(const QString &address, const QString &label, const QString &email, const QString &url, int status);
    void refreshContactTable(void);

    friend class ContactTablePriv;
};

#endif // CONTACTTABLEMODEL_H
