/**
*CVS Log info
*$Log$
*Revision 1.1.2.1  2004/04/28 03:45:49  ddiego
*migration towards new directory structure
*
*Revision 1.9.8.1  2004/04/26 21:58:44  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.9  2003/05/17 20:37:13  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.8.16.2  2003/03/23 03:23:51  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.8.16.1  2003/03/12 03:11:29  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.8  2002/05/09 03:10:43  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.7.4.1  2002/03/22 17:32:31  zzack
*changed to new include style
*
*Revision 1.7  2002/01/24 01:46:48  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/

/**
*Copyright (c) 2000-2001, Jim Crafton
*All rights reserved.
*Redistribution and use in source and binary forms, with or without
*modification, are permitted provided that the following conditions
*are met:
*	Redistributions of source code must retain the above copyright
*	notice, this list of conditions and the following disclaimer.
*
*	Redistributions in binary form must reproduce the above copyright
*	notice, this list of conditions and the following disclaimer in 
*	the documentation and/or other materials provided with the distribution.
*
*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
*AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS
*OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
*SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*NB: This software will not save the world.
*/

// Win32SocketPeer.h

#ifndef _VCF_WIN32SOCKETPEER_H__
#define _VCF_WIN32SOCKETPEER_H__

namespace VCFNet
{

class SocketObject;

class Socket;

#define MAX_PENDING_CONNECTS SOMAXCONN

class Win32SocketPeer : public SocketPeer {
public:
	Win32SocketPeer( Socket* socket, const VCF::String& host, const int& port );

	Win32SocketPeer( Socket* socket, const int& socketPeerID, const VCF::String& host, const int& port );

	virtual ~Win32SocketPeer();

	virtual SocketState startListening();

	virtual int send( const char* bytes, const int& size );
	

	virtual SocketState stopListening();

	virtual void setPort( const int& port );

	virtual int getPort();
	
	virtual void setHost( const VCF::String& host );

	virtual VCF::String getHost();

	static void startup();

	SocketState connectTo( const VCF::String& hostName, const int& port );

	virtual bool hasWaitingClients();

	virtual Socket* getClient();

	virtual void checkForPendingData( const int& waitAndBlock );

	virtual int getSocketPeerID();

	static VCF::String getLocalMachineName();
protected:
	void readDataFromClient( Socket* client, fd_set* readfd );
private:
	struct sockaddr_in sin_;
	std::vector<Socket*> connectedClients_;
	std::vector<Socket*> disconnectedClients_;
	Socket* socket_;
	VCF::String host_;
	int port_;
	SOCKET  sock_;
	static bool WinSockStarted;
};


};


#endif // _VCF_WIN32SOCKETPEER_H__


