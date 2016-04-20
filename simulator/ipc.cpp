#include <ipc.h>
#include <sstream>
#include <messages/genericconsolestringoutputmessage.h>
#include <outputqueuemanager.h>
#include <outputmessage.h>

using namespace std;

OutputQueueManager mOutQueue; // Use Binary Format

extern bool use_gui;

void initIpcQueue()
{
    if (!use_gui)
        mOutQueue.disableIpcQueue();

    mOutQueue.start();
}

void guiSendCurrentStep (unsigned int tstep)
{
    if (use_gui) {
        ostringstream ss;
        ss << "=S" << tstep << endl;      /* use gui */
        mOutQueue.enqueue(boost::shared_ptr<OutputMessage>(new GenericConsoleStringOutputMessage(ss.str())));
    }
}

void guiSendUpdateCommand (const std::string &filename, int tstep)
{
    if (use_gui) {
        ostringstream ss;
        ss << "=U" << filename << " " << tstep << endl;
        mOutQueue.enqueue(boost::shared_ptr<OutputMessage>(new GenericConsoleStringOutputMessage(ss.str())));
    }
}

void guiSendMemoryInfo(const MemoryInfo &info)
{
    if (use_gui) {
        ostringstream ss;
        ss << "=Dm" << info.rss() << " " << info.peakRss() << endl;
        mOutQueue.enqueue(boost::shared_ptr<OutputMessage>(new GenericConsoleStringOutputMessage(ss.str())));
    }

}

void guiSendCapture(bool on)
{
    if (use_gui) {
        ostringstream ss;
        ss << "=Dc" << (on ? "+" : "-") << endl;
        mOutQueue.enqueue(boost::shared_ptr<OutputMessage>(new GenericConsoleStringOutputMessage(ss.str())));
    }
}

void guiSendTerminalMessage(const string &ss)
{
    if (use_gui)
        mOutQueue.enqueue(boost::shared_ptr<OutputMessage>(new GenericConsoleStringOutputMessage(ss)));
}

void finalizeIpcQueue()
{
    mOutQueue.finish();
}