#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include <QScrollBar>
#include <QTextEdit>
#include <QPainter>
#include <QSplitter>
#include "qt_example.h"
#include "message/Message.h"
#include "system/SetupSystem.h"
#include "util/MiscUtilityFunctions.h"  // for ParseConnectArg()
#include "util/Hashtable.h"

ExampleWidget :: ExampleWidget(ExampleWindow * master) : _master(master), _localPosition(0.5f, 0.75f), _isMousePressed(false)
{
   setMinimumSize(200, 20);
}

void ExampleWidget :: paintEvent(QPaintEvent *)
{
   QPainter p(this);

   if (_master->_isConnected)
   {
      p.fillRect(QRect(0,0,width(),height()), Qt::darkGray);

      p.setPen(Qt::gray);
      DrawText(p, NormalizedToQtCoords(Point(0.5f, 0.5f)), "Click and drag here!");

      p.setPen(Qt::black);
      for (HashtableIterator<String, MessageRef> iter(_master->_states); iter.HasData(); iter++) DrawUser(p, iter.GetValue());

      p.setPen(Qt::green);
      DrawUser(p, _master->_localState);
   }
   else 
   {
      p.fillRect(QRect(0,0,width(),height()), Qt::lightGray);

      p.setPen(Qt::darkGray);
      DrawText(p, NormalizedToQtCoords(Point(0.5f, 0.5f)), "(Not currently connected to server)");
   }
}

void ExampleWidget :: DrawUser(QPainter & p, const MessageRef & data)
{
   if (data()) DrawText(p, NormalizedToQtCoords(data()->GetPoint("position")), data()->GetString("username")());
}

void ExampleWidget :: DrawText(QPainter & p, const QPoint & pt, const QString & text)
{
   // draw the text centered about (pt)
   p.drawText(QPoint(pt.x()-(p.fontMetrics().width(text)/2), pt.y()), text);
}

void ExampleWidget :: mousePressEvent(QMouseEvent * e)
{
   QWidget::mousePressEvent(e);
   _isMousePressed = true;
   SetLocalPosition(QtCoordsToNormalized(e->pos()));
}

void ExampleWidget :: mouseMoveEvent(QMouseEvent * e)
{
   QWidget::mouseMoveEvent(e);
   if (_isMousePressed) SetLocalPosition(QtCoordsToNormalized(e->pos()));
}

void ExampleWidget :: mouseReleaseEvent(QMouseEvent * e)
{
   QWidget::mouseReleaseEvent(e);
   _isMousePressed = false;
}

void ExampleWidget :: SetLocalPosition(const Point & normPt)
{
   _localPosition = normPt;
   emit LocalPositionChanged();
   update();
}

Point ExampleWidget :: QtCoordsToNormalized(const QPoint & pt) const
{
   return Point(((float)pt.x())/width(), ((float)pt.y())/height());
}

QPoint ExampleWidget :: NormalizedToQtCoords(const Point & pt) const
{
   return QPoint((int)((pt.x()*width())+0.5f), (int)((pt.y()*height())+0.5f));
}

ExampleWindow :: ExampleWindow() : _isConnected(false), _curUserName("Anonymous")
{
   resize(640, 400);

   QBoxLayout * vbl = new QBoxLayout(QBoxLayout::TopToBottom, this);
   vbl->setSpacing(3);
   vbl->setMargin(2);

   QWidget * topRow = new QWidget;
   {
      QBoxLayout * topRowLayout = new QBoxLayout(QBoxLayout::LeftToRight, topRow);
      topRowLayout->setSpacing(6);
      topRowLayout->setMargin(2);

      topRowLayout->addWidget(new QLabel("Server:"));

      _serverName = new QLineEdit;
      _serverName->setText("localhost:2960");
      connect(_serverName, SIGNAL(returnPressed()), this, SLOT(ConnectToServer()));
      connect(_serverName, SIGNAL(editingFinished()), this, SLOT(ConnectToServer()));
      topRowLayout->addWidget(_serverName, 1);

      _connectButton = new QPushButton("Connect to Server");
      connect(_connectButton, SIGNAL(clicked()), this, SLOT(ConnectToServer()));
      topRowLayout->addWidget(_connectButton);

      _disconnectButton = new QPushButton("Disconnect from Server");
      connect(_disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectFromServer()));
      topRowLayout->addWidget(_disconnectButton);
   }
   vbl->addWidget(topRow);

   QSplitter * splitter = new QSplitter;
   splitter->setOrientation(Qt::Vertical);
   {
      _exampleWidget = new ExampleWidget(this);
      connect(_exampleWidget, SIGNAL(LocalPositionChanged()), this, SLOT(UploadLocalState()));
      splitter->addWidget(_exampleWidget);

      QWidget * splitBottom = new QWidget;
      {
         QBoxLayout * splitBottomLayout = new QBoxLayout(QBoxLayout::TopToBottom, splitBottom);
         splitBottomLayout->setMargin(2);
         splitBottomLayout->setSpacing(2);
 
         _chatText = new QTextEdit;
         _chatText->setReadOnly(true);
         splitBottomLayout->addWidget(_chatText, 1);

         QWidget * botRow = new QWidget;
         {
            QBoxLayout * botRowLayout = new QBoxLayout(QBoxLayout::LeftToRight, botRow);
            botRowLayout->setSpacing(3);
            botRowLayout->setMargin(3);

            _userName = new QLineEdit;
            _userName->setText(_curUserName);
            _userName->setMinimumWidth(100);
            connect(_userName, SIGNAL(editingFinished()), this, SLOT(UserChangedName()));
            connect(_userName, SIGNAL(returnPressed()), this, SLOT(UserChangedName()));
            botRowLayout->addWidget(_userName);
            botRowLayout->addWidget(new QLabel(":"));
       
            _chatEntry = new QLineEdit;
            connect(_chatEntry, SIGNAL(returnPressed()), this, SLOT(SendChatText()));
            botRowLayout->addWidget(_chatEntry, 1);
         }
         splitBottomLayout->addWidget(botRow);
      }
      splitter->addWidget(splitBottom);
   }
   vbl->addWidget(splitter);
   
   connect(&_mtt, SIGNAL(SessionConnected(const String &, const IPAddressAndPort &)), this, SLOT(SessionConnected()));
   connect(&_mtt, SIGNAL(MessageReceived(const MessageRef &, const String &)), this, SLOT(MessageReceived(const MessageRef &)));
   connect(&_mtt, SIGNAL(SessionDisconnected(const String &)), this, SLOT(SessionDisconnected()));
   UpdateButtons();

   ConnectToServer();  // we might as well connect automatically on startup
}

ExampleWindow :: ~ExampleWindow()
{
   _mtt.ShutdownInternalThread();
}

void ExampleWindow :: UserChangedName()
{
   if (_userName->text() != _curUserName)
   {
      _curUserName = _userName->text();
      UploadLocalState();
      _exampleWidget->update();
   }
}

void ExampleWindow :: UpdateButtons()
{
   _chatEntry->setEnabled(_isConnected);
   _connectButton->setEnabled(!_isConnected);
   _disconnectButton->setEnabled(_isConnected);
   _exampleWidget->setEnabled(_isConnected);
   _exampleWidget->update();
}

static String FromQ(const QString & s)
{
   return String(s.toUtf8().constData());
}

void ExampleWindow :: ConnectToServer()
{
   _isConnected = false;
   _mtt.Reset();

   String hostname;
   uint16 port = 2960;  // default port for muscled
   if (ParseConnectArg(FromQ(_serverName->text()), hostname, port) != B_NO_ERROR) 
   {
      AddChatText(QString("Unable to parse server name %1.").arg(_serverName->text()));
   }
   else if ((_mtt.AddNewConnectSession(hostname, port, false, SecondsToMicros(1)) == B_NO_ERROR)&&(_mtt.StartInternalThread() == B_NO_ERROR))
   {
      AddChatText(QString("Connecting to server %1...").arg(_serverName->text()));
   }
   else AddChatText(QString("Could not initiate connection to server %1.").arg(_serverName->text()));

   UpdateButtons();
}

void ExampleWindow :: DisconnectFromServer()
{
   _isConnected = false;
   _mtt.Reset();
   AddChatText("Disconnected from server.");
   UpdateButtons();
}

void ExampleWindow :: SessionConnected()
{
   _isConnected = true;
   UpdateButtons();

   // Subscribe to the data published by other ExampleClients
   MessageRef subscribeMsg = GetMessageFromPool(PR_COMMAND_SETPARAMETERS);
   subscribeMsg()->AddBool("SUBSCRIBE:qt_example/state", true);
   _mtt.SendMessageToSessions(subscribeMsg);

   // And upload our current state to the server
   UploadLocalState();

   AddChatText(QString("Connected to server %1").arg(_serverName->text()));
}

void ExampleWindow :: AddChatText(const QString & text)
{
   _chatText->append(text);
   _chatText->verticalScrollBar()->setValue(_chatText->verticalScrollBar()->maximum());
}

void ExampleWindow :: UploadLocalState()
{
   MessageRef stateMsg = GetMessageFromPool();
   stateMsg()->AddString("username", FromQ(_curUserName));
   stateMsg()->AddPoint("position", _exampleWidget->GetLocalPosition());

   MessageRef uploadMsg = GetMessageFromPool(PR_COMMAND_SETDATA);
   uploadMsg()->AddMessage("qt_example/state", stateMsg);

   _localState = stateMsg;
   if (_isConnected) _mtt.SendMessageToSessions(uploadMsg);
}

enum {
   QT_EXAMPLE_CHAT_TEXT = 6666  // arbitrary
};

void ExampleWindow :: MessageReceived(const MessageRef & msg)
{
   switch(msg()->what)
   {
      case QT_EXAMPLE_CHAT_TEXT:
      {
         String fromUser = msg()->GetString("username");
         String text     = msg()->GetString("text");
         AddChatText(QString("[%1] said: %2").arg(fromUser()).arg(text()));
      }
      break;

      case PR_RESULT_DATAITEMS:
      {
         // Look for strings that indicate that subscribed nodes were removed from the tree
         {
            const String * nodePath;
            for (int i=0; (msg()->FindString(PR_NAME_REMOVED_DATAITEMS, i, &nodePath) == B_NO_ERROR); i++)
            {
               MessageRef existingState;
               if (_states.Remove(*nodePath, existingState) == B_NO_ERROR)
               {
                  AddChatText(QString("[%1] has disconnected from the server.").arg(existingState()->GetString("username")()));
                  _exampleWidget->update();
               }
            }
         }

         // And then look for sub-messages that represend subscribed nodes that were added to the tree, or updated in-place
         {
            for (MessageFieldNameIterator iter = msg()->GetFieldNameIterator(B_MESSAGE_TYPE); iter.HasData(); iter++)
            {
               MessageRef data;
               for (uint32 i=0; msg()->FindMessage(iter.GetFieldName(), i, data) == B_NO_ERROR; i++)
               {
                  if (_states.ContainsKey(iter.GetFieldName()) == false) AddChatText(QString("[%1] has connected to the server.").arg(data()->GetString("username")()));
                  _states.Put(iter.GetFieldName(), data);
               }
               _exampleWidget->update();
            }
         }
      }
      break;
   }
}

void ExampleWindow :: SendChatText()
{
   QString text = _chatEntry->text();
   _chatEntry->setText(QString::null);

   MessageRef chatMsg = GetMessageFromPool(QT_EXAMPLE_CHAT_TEXT);
   chatMsg()->AddString("username", FromQ(_curUserName));  // tag Message with who sent it
   chatMsg()->AddString("text", FromQ(text));
   chatMsg()->AddString(PR_NAME_KEYS, "qt_example");  // make sure the chat text only goes to qt_example clients on the server
   MessageReceived(chatMsg);  // handle it locally also (the server won't send it back to us, by default)

   _mtt.SendMessageToSessions(chatMsg);
}

void ExampleWindow :: SessionDisconnected()
{
   _isConnected = false;
   UpdateButtons();

   _states.Clear();
   _exampleWidget->update();

   AddChatText("Disconnected from server!");
}

int main(int argc, char ** argv)
{
   CompleteSetupSystem css;  // necessary for MUSCLE initialization
   QApplication app(argc, argv);
   ExampleWindow fw;
   fw.show();
   return app.exec();
}
