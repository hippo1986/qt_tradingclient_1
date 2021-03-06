/*
 * File:   ReqMktData.cpp
 * Author: Piotr Gregor  postmaster@cf16.eu
 *
 * Created on May 22, 2013, 8:36 PM
 */

#include "ReqMktDataGUI.h"
#include "TradingStrategies/Cointegration.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <typeinfo>
#include <ql/patterns/observable.hpp>
    
ReqMktDataGUI::ReqMktDataGUI(boost::shared_ptr<IB::PosixClient> client_ptr, QWidget* parent):client_(client_ptr),QDialog(parent),thisGUIReqActive_(false){
    
    widget_.setupUi(this);
    
    //tickerId: use next possible value based on existing gui requests
    widget_.lineEdit_Id->setText(QString::number(totalGUIReqActive+1));
    //contract.symbol
    widget_.lineEdit_Symbol->setText("eur");
    //contract.secType
    widget_.lineEdit_Type->setText("cash");
    //contract.exchange
    widget_.lineEdit_Exchange->setText("idealpro");
    //contract.currency
    widget_.lineEdit_Currency->setText("usd");
    //generic tick tags
    widget_.lineEdit_genericTickTags->setText("233");
    
    QObject::connect(widget_.requestButton, SIGNAL(clicked()), this, SLOT(requestClicked()));
    QObject::connect(widget_.cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(widget_.guiRequestButton, SIGNAL(clicked()), this, SLOT(guiRequestClicked()));
    //this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this,SIGNAL(newMktData(int,rec_ptr)),this,SLOT(displayData(int,rec_ptr)));
}

ReqMktDataGUI::~ReqMktDataGUI() {
   #ifdef DEBUG 
        printf( "I am dead!\n");
   #endif
        if(thisGUIReqActive_){
            cancelClicked();
        }
}

void ReqMktDataGUI::myTickPriceUpdate(int tickerId, rec_ptr record_ptr){
    try{
        //const IBAdditions::TickPriceRecord* tickPriceRecord = dynamic_cast<const IBAdditions::TickPriceRecord*>(record.get());
        tickPriceRec_ptr tickPriceRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickPriceRecord>(record_ptr));
        #ifdef DEBUG 
           printf( "[ReqMktDataGUI::myTickPriceUpdate] %s Id: %d, price: %f, tickType: %d\n",IBAdditions::ibTickTypeToStdString(tickPriceRecord_ptr->tickType_).c_str(), tickerId, tickPriceRecord_ptr->price_,tickPriceRecord_ptr->tickType_);
           
           IBAdditions::ContractEvent contractEvent_ = IBAdditions::ContractEvent(observedContracts_[tickerId], tickPriceRecord_ptr->event_);
           printf("[ReqMktDataGUI::myTickPriceUpdate] vector->size: %d\n", marketDataRepository[contractEvent_].size());
           printf("[ReqMktDataGUI::myTickPriceUpdate] repo->size: %d\n", marketDataRepository.contractEventCount());
        #endif
//        QString qs=QString("myTickPriceUpdate! Id: %1, price: %2, tickType: %3").arg(tickerId).arg(tickPriceRecord_ptr->price_).arg(tickPriceRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
           printf( "[ReqMktDataGUI::myTickPriceUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

/**
 * 
 * @param tickerId the identifier to mktDataRequest
 * @param record_ptr incoming data
 * TODO: signal mutex conditional variable here so 
 * each strategy will be recalculated
 */
void ReqMktDataGUI::myTickSizeUpdate(int tickerId, rec_ptr record_ptr){
    try{
        tickSizeRec_ptr tickSizeRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickSizeRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "[ReqMktDataGUI::myTickSizeUpdate] %s Id: %d, size: %d, tickType: %d\n",IBAdditions::ibTickTypeToStdString(tickSizeRecord_ptr->tickType_).c_str(), tickerId, tickSizeRecord_ptr->size_,tickSizeRecord_ptr->tickType_);
        IBAdditions::ContractEvent contractEvent_ = IBAdditions::ContractEvent(observedContracts_[tickerId], tickSizeRecord_ptr->event_);
        boost::shared_ptr<IBAdditions::TickSizeRecord> repoRecord = boost::dynamic_pointer_cast<IBAdditions::TickSizeRecord>
                (marketDataRepository[contractEvent_].back());
        
        printf("[ReqMktDataGUI::myTickSizeUpdate] vector->size: %d\n", marketDataRepository[contractEvent_].size());
        printf("[ReqMktDataGUI::myTickSizeUpdate] repo->size: %d\n", marketDataRepository.contractEventCount());
        int size = repoRecord->size_;
        printf("[ReqMktDataGUI::myTickSizeUpdate] repoRecord->size_: %d\n", size);
    #endif
//        QString qs=QString("myTickSizeUpdate! Id: %1, size: %2, tickType: %3").arg(tickerId).arg(tickSizeRecord_ptr->size_).arg(tickSizeRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
        //widget.textEdit_dataFeed->append("myTickSizeUpdate something...");
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "[ReqMktDataGUI::myTickSizeUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}
void ReqMktDataGUI::myTickStringUpdate(int tickerId, rec_ptr record_ptr){
        try{
        tickStringRec_ptr tickStringRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickStringRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "[ReqMktDataGUI::myTickStringUpdate] %s Id: %d, string: %s, tickType: %d\n",IBAdditions::ibTickTypeToStdString(tickStringRecord_ptr->tickType_).c_str(), tickerId, tickStringRecord_ptr->string.c_str(),tickStringRecord_ptr->tickType_);
    #endif
//        QString qs=QString("myTickStringUpdate! Id: %1, string: ").arg(tickerId)+QString::fromStdString(tickStringRecord_ptr->string);
//        qs+=QString(" tickType: %1").arg(tickStringRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "[ReqMktDataGUI::myTickStringUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

void ReqMktDataGUI::myTickPriceGUIUpdate(int tickerId, rec_ptr record_ptr){
    try{
        //const IBAdditions::TickPriceRecord* tickPriceRecord = dynamic_cast<const IBAdditions::TickPriceRecord*>(record.get());
        tickPriceRec_ptr tickPriceRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickPriceRecord>(record_ptr));
        #ifdef DEBUG 
           printf( "[myTickPriceGUIUpdate] Id: %d, price: %f, tickType: %d\n",tickerId,tickPriceRecord_ptr->price_,tickPriceRecord_ptr->tickType_);
           IBAdditions::ContractEvent contractEvent_ = IBAdditions::ContractEvent(guiObservedContracts_[tickerId], tickPriceRecord_ptr->event_);
           printf("[ReqMktDataGUI::myTickPriceGUIUpdate] vector->size: %d\n", marketDataRepository[contractEvent_].size());
           printf("[ReqMktDataGUI::myTickPriceGUIUpdate] repo->size: %d\n", marketDataRepository.contractEventCount());
        #endif
        QString qs=QString::fromStdString(IBAdditions::ibTickTypeToStdString(tickPriceRecord_ptr->tickType_)) +
                QString(" TickPrice Id:%1, price:%2, tickType:%3").arg(tickerId).arg(tickPriceRecord_ptr->price_).arg(tickPriceRecord_ptr->tickType_);
        widget_.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
           printf( "[myTickPriceGUIUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

/**
 * 
 * @param tickerId the identifier to mktDataRequest
 * @param record_ptr incoming data
 */
void ReqMktDataGUI::myTickSizeGUIUpdate(int tickerId, rec_ptr record_ptr){
    try{
        tickSizeRec_ptr tickSizeRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickSizeRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "[myTickSizeGUIUpdate] Id: %d, size: %d, tickType: %d\n",tickerId,tickSizeRecord_ptr->size_,tickSizeRecord_ptr->tickType_);
        IBAdditions::ContractEvent contractEvent_ = IBAdditions::ContractEvent(guiObservedContracts_[tickerId], tickSizeRecord_ptr->event_);
        boost::shared_ptr<IBAdditions::TickSizeRecord> repoRecord = boost::dynamic_pointer_cast<IBAdditions::TickSizeRecord>
                (marketDataRepository[contractEvent_].back());
        printf("[ReqMktDataGUI::myTickSizeGUIUpdate] vector->size: %d\n", marketDataRepository[contractEvent_].size());
        printf("[ReqMktDataGUI::myTickSizeGUIUpdate] repo->size: %d\n", marketDataRepository.contractEventCount());
        int size = repoRecord->size_;
        printf("[ReqMktDataGUI::myTickSizeGUIUpdate] repoRecord->size_: %d\n", size);
    #endif
        QString qs=QString::fromStdString(IBAdditions::ibTickTypeToStdString(tickSizeRecord_ptr->tickType_)) +
                QString(" TickSize Id:%1, size:%2, tickType:%3").arg(tickerId).arg(tickSizeRecord_ptr->size_).arg(tickSizeRecord_ptr->tickType_);
        widget_.textEdit_dataFeed->append(qs);
        //widget.textEdit_dataFeed->append("myTickSizeGUIUpdate something...");
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "[myTickSizeGUIUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

void ReqMktDataGUI::myTickStringGUIUpdate(int tickerId, rec_ptr record_ptr){
        try{
        tickStringRec_ptr tickStringRecord_ptr(boost::dynamic_pointer_cast<IBAdditions::TickStringRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "[myTickStringGUIUpdate] Id: %d, string: %s, tickType: %d\n",tickerId,tickStringRecord_ptr->string.c_str(),tickStringRecord_ptr->tickType_);
        IBAdditions::ContractEvent contractEvent_ = IBAdditions::ContractEvent(guiObservedContracts_[tickerId], tickStringRecord_ptr->event_);
           printf("[ReqMktDataGUI::myTickStringGUIUpdate] vector->size: %d\n", marketDataRepository[contractEvent_].size());
           printf("[ReqMktDataGUI::myTickStringGUIUpdate] repo->size: %d\n", marketDataRepository.contractEventCount());
    #endif
        QString qs=QString::fromStdString(IBAdditions::ibTickTypeToStdString(tickStringRecord_ptr->tickType_)) +
                QString(" TickString Id:%1, string:").arg(tickerId)+QString::fromStdString(tickStringRecord_ptr->string);
        qs+=QString(" tickType:%1").arg(tickStringRecord_ptr->tickType_);
        widget_.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "[myTickStringGUIUpdate] badCast for tickerId: %d\n",tickerId);
        #endif
    }
}
//public slots
void ReqMktDataGUI::requestClicked(){
    
    /* note: if tickerId is not unique MarketData
     * will not be inserted into marketDataFeed map of client_
     * and Repository won't be resized (records coming for this
     * tickerId will be stored for previous ContractEvent
     * associated with this tickerId
     * TODO: change map to multimap or assert unique tickerId
     */
    IB::Contract contract;
    contract.symbol = widget_.lineEdit_Symbol->text().toStdString();
    contract.secType = widget_.lineEdit_Type->text().toStdString();
    contract.exchange = widget_.lineEdit_Exchange->text().toStdString();
    contract.currency = widget_.lineEdit_Currency->text().toStdString();
    contract.localSymbol = widget_.lineEdit_LocalSymbol->text().toStdString();
    
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<MarketData> tickPriceMktData(new MarketData(IBAdditions::TickPrice,widget_.lineEdit_Id->text().toInt(),contract));
    // create tickPrice event observer and push it into vector stored in this GUI form
    tickPriceObservers_.push_back(boost::shared_ptr<MarketDataObserver>(
            new MarketDataObserver(tickPriceMktData,IBAdditions::TickPrice,boost::bind(&ReqMktDataGUI::myTickPriceUpdate,this,_1,_2))));
    // put this connection into tickerIdMarketDataMap, it will be stored in tickPriceMarketDataFeed
    client_->marketDataFeedInsert(tickPriceMktData);
    
    
    // also register for tickSize updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<MarketData> tickSizeMktData(new MarketData(IBAdditions::TickSize,widget_.lineEdit_Id->text().toInt(),contract));    
    // create tickSize event observer and push it into vector stored in this GUI form
    tickSizeObservers_.push_back(boost::shared_ptr<MarketDataObserver>(
            new MarketDataObserver(tickSizeMktData,IBAdditions::TickSize,boost::bind(&ReqMktDataGUI::myTickSizeUpdate,this,_1,_2))));
    // put this connection into tickerIdMarketDataMap, it will be stored in tickSizeMarketDataFeed
    client_->marketDataFeedInsert(tickSizeMktData);
    
    
    // and register also for tickString updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<MarketData> tickStringMktData(new MarketData(IBAdditions::TickString,widget_.lineEdit_Id->text().toInt(),contract));    
    // create tickString event observer and push it into vector stored in this GUI form
    tickStringObservers_.push_back(boost::shared_ptr<MarketDataObserver>(
            new MarketDataObserver(tickStringMktData,IBAdditions::TickString,boost::bind(&ReqMktDataGUI::myTickStringUpdate,this,_1,_2))));
    // put this connection into tickerIdMarketDataMap, it will be stored in tickStringMarketDataFeed
    client_->marketDataFeedInsert(tickStringMktData);    
    
    
    client_->reqMktData(widget_.lineEdit_Symbol->text().toStdString(), widget_.lineEdit_Type->text().toStdString(),
        widget_.lineEdit_Exchange->text().toStdString(), widget_.lineEdit_Currency->text().toStdString(), 
            widget_.lineEdit_Id->text().toInt(), widget_.lineEdit_genericTickTags->text().toStdString(), 
            widget_.lineEdit_LocalSymbol->text().toStdString(), widget_.checkBox_Snapshot->isChecked());
    observedContracts_.insert(std::pair<int, IB::Contract>(widget_.lineEdit_Id->text().toInt(),contract));
}

void ReqMktDataGUI::cancelClicked() {
    if (thisGUIReqActive_) {
        if(client_->isConnected()){
            guiMarketDataFeedDelete();
            marketDataFeedDelete();
        }
//        if (totalGUIReqActive == 1) {
//            endProcessMessages();
//        }
        thisGUIReqActive_ = false;
        totalGUIReqActive--;
    } else {
        marketDataFeedDelete();
    }
} 

void ReqMktDataGUI::marketDataFeedDelete(void){
//    for(vecPmktDataObsIt it=tickPriceObservers.begin();it!=tickPriceObservers.end();it++){
//        client_->cancelMktData((*it)->get_pMktDataObservable()->getTickerId());
//        (*it)->unregisterWithAll();
//        tickPriceObservers.erase(it);
//    }
//    
//    for(vecPmktDataObsIt it=tickSizeObservers.begin();it!=tickSizeObservers.end();it++){
//        client_->cancelMktData((*it)->get_pMktDataObservable()->getTickerId());
//        (*it)->unregisterWithAll();
//        tickSizeObservers.erase(it);
//    }
//
//    for(vecPmktDataObsIt it=tickStringObservers.begin();it!=tickStringObservers.end();it++){
//        client_->cancelMktData((*it)->get_pMktDataObservable()->getTickerId());
//        (*it)->unregisterWithAll();
//        tickStringObservers.erase(it);
//    }
    tickerIdContractMap::iterator it = observedContracts_.begin();
    while(it!=observedContracts_.end()){
        printf("\nReqMktdataGUI::marketDataFeedDelete: canceling mkt data requests with id %d\n",(*it).first);
        client_->cancelMktData((*it).first);
        client_->marketDataFeedDelete((*it).first);
        observedContracts_.erase(it++);
    }
    tickPriceObservers_.clear();
    tickSizeObservers_.clear();
    tickStringObservers_.clear();
}

void ReqMktDataGUI::guiMarketDataFeedDelete(void){
    tickerIdContractMap::iterator it = guiObservedContracts_.begin();
    while(it!=guiObservedContracts_.end()){
        printf("\n[ReqMktdataGUI::guiMarketDataFeedDelete] canceling mkt data request with id %d\n",(*it).first);
        client_->cancelMktData((*it).first);
        client_->guiMarketDataFeedDelete((*it).first);
        guiObservedContracts_.erase(it++);
    }
}

void ReqMktDataGUI::displayData(int tickerId, rec_ptr record_ptr){
    printf( "[ReqMktDataGUI::displayData] for tickerId: %d\n",tickerId);
    widget_.textEdit_dataFeed->append("something...");
}

void ReqMktDataGUI::guiRequestClicked(){
    
    /* note: if tickerId is not unique GUIMarketData
     * will not be inserted into marketDataFeed map of client_
     * and Repository won't be resized (records coming for this
     * tickerId will be stored for previous ContractEvent
     * associated with this tickerId
     * TODO: change map to multimap or assert unique tickerId
     */
    IB::Contract contract;
    contract.symbol = widget_.lineEdit_Symbol->text().toStdString();
    contract.secType = widget_.lineEdit_Type->text().toStdString();
    contract.exchange = widget_.lineEdit_Exchange->text().toStdString();
    contract.currency = widget_.lineEdit_Currency->text().toStdString();
    contract.localSymbol = widget_.lineEdit_LocalSymbol->text().toStdString();
    
    // register for tickPrice updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<GUIMarketData> tickPriceGUIMktData(new GUIMarketData(IBAdditions::TickPrice,widget_.lineEdit_Id->text().toInt(),contract));    
    // connect slot to signal
    QObject::connect(tickPriceGUIMktData.get(), SIGNAL(newRecord(int, rec_ptr)), this, SLOT(myTickPriceGUIUpdate(int, rec_ptr)), Qt::QueuedConnection);
    // put this connection into tickerIdGUIMarketDataMap, it will be stored in tickPriceGUIMarketDataFeed
    client_->guiMarketDataFeedInsert(tickPriceGUIMktData);
    
    // register for tickSize updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<GUIMarketData> tickSizeGUIMktData(new GUIMarketData(IBAdditions::TickSize,widget_.lineEdit_Id->text().toInt(),contract));    
    // connect slot to signal
    QObject::connect(tickSizeGUIMktData.get(), SIGNAL(newRecord(int, rec_ptr)), this, SLOT(myTickSizeGUIUpdate(int, rec_ptr)), Qt::QueuedConnection);
    // put this connection into marketDataFeed map, it will be stored in tickSizeMarketDataFeed
    client_->guiMarketDataFeedInsert(tickSizeGUIMktData);   
    
    // register for tickString updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<GUIMarketData> tickStringGUIMktData(new GUIMarketData(IBAdditions::TickString,widget_.lineEdit_Id->text().toInt(),contract));    
    // connect slot to signal
    QObject::connect(tickStringGUIMktData.get(), SIGNAL(newRecord(int, rec_ptr)), this, SLOT(myTickStringGUIUpdate(int, rec_ptr)), Qt::QueuedConnection);
    // put this connection into marketDataFeed map, it will be stored in tickSizeMarketDataFeed
    client_->guiMarketDataFeedInsert(tickStringGUIMktData);    
   
    //TODO: client->reqMktData has to take all parameters of contract specified in GUI
    // now we process only few of them
    client_->reqMktData(contract.symbol, contract.secType, contract.exchange, contract.currency, 
            widget_.lineEdit_Id->text().toInt(), widget_.lineEdit_genericTickTags->text().toStdString(), 
            contract.localSymbol, widget_.checkBox_Snapshot->isChecked());
    guiObservedContracts_.insert(std::pair<int, IB::Contract>(widget_.lineEdit_Id->text().toInt(),contract));
    
    thisGUIReqActive_=true;
    totalGUIReqActive++;
//    if(totalGUIReqActive==1){
//        processMessages();
//    }
}
