/* 
 * File:   TickerDisplayModel.h
 * Author: Piotr Gregor  postmaster@cf16.eu
 *
 * Created on September 5, 2013, 8:24 PM
 */

#ifndef TICKERDISPLAYMODEL_H
#define	TICKERDISPLAYMODEL_H

#include <IBAdditions/IB_events.h>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QtCore>

#include <vector>

/**
 * model which allows for display 
 * of available data streams as tickers
 */
class TickerDisplayModel : public QStandardItemModel {
    
Q_OBJECT
 public:
     TickerDisplayModel(QObject *parent);
     int rowCount(const QModelIndex &parent = QModelIndex()) const ;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
     bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
     Qt::ItemFlags flags(const QModelIndex & index) const;
 private:
     int m_;
     int n_;
     std::vector<IBAdditions::ContractEvent>& availableTickers;
 signals:
     void editCompleted(const QString &);
};

#endif	/* TICKERDISPLAYMODEL_H */

