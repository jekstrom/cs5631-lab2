#ifndef QT_EXAMPLE_H
#define QT_EXAMPLE_H

#include <QWidget>
#include "qtsupport/QMessageTransceiverThread.h"
#include "support/Point.h"

using namespace muscle;

class QLineEdit;
class QTextEdit;
class QPainter;
class QPushButton;
class ExampleWindow;

class ExampleWidget : public QWidget
{
Q_OBJECT

public:
   ExampleWidget(ExampleWindow * master);

   virtual void paintEvent(QPaintEvent * e);
   virtual void mousePressEvent(QMouseEvent * e);
   virtual void mouseMoveEvent(QMouseEvent * e);
   virtual void mouseReleaseEvent(QMouseEvent * e);

   const Point & GetLocalPosition() const {return _localPosition;}

signals:
   void LocalPositionChanged();

private:
   Point QtCoordsToNormalized(const QPoint & pt) const;
   QPoint NormalizedToQtCoords(const Point & pt) const;
   void DrawUser(QPainter & p, const MessageRef & data);
   void DrawText(QPainter & p, const QPoint & pt, const QString & text);
   void SetLocalPosition(const Point & p);

   ExampleWindow * _master;
   Point _localPosition;
   bool _isMousePressed;
};

class ExampleWindow : public QWidget
{
Q_OBJECT

public:
   ExampleWindow();
   virtual ~ExampleWindow();

private slots:
   // slots for responding to GUI actions
   void ConnectToServer();
   void DisconnectFromServer();
   void UpdateButtons();
   void UploadLocalState();
   void UserChangedName();
   void AddChatText(const QString & text);
   void SendChatText();

   // slots called by signals from the QMessageTransceiverThread object
   void SessionConnected();
   void MessageReceived(const MessageRef & msg);
   void SessionDisconnected();

private:
   friend class ExampleWidget;

   bool _isConnected;

   QPushButton * _connectButton;
   QPushButton * _disconnectButton;
   QLineEdit * _serverName;

   ExampleWidget * _exampleWidget;

   QTextEdit * _chatText;
   QLineEdit * _userName;
   QLineEdit * _chatEntry;

   QString _curUserName;

   QMessageTransceiverThread _mtt;

   MessageRef _localState;  // contains our local state, as recently uploaded
   Hashtable<String, MessageRef> _states;  // contains info we gathered about other clients, from our subscription
};

#endif
