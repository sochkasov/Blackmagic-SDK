/* -LICENSE-START-
 ** Copyright (c) 2010 Blackmagic Design
 **
 ** Permission is hereby granted, free of charge, to any person or organization
 ** obtaining a copy of the software and accompanying documentation covered by
 ** this license (the "Software") to use, reproduce, display, distribute,
 ** execute, and transmit the Software, and to prepare derivative works of the
 ** Software, and to permit third-parties to whom the Software is furnished to
 ** do so, all subject to the following:
 ** 
 ** The copyright notices in the Software and this entire statement, including
 ** the above license grant, this restriction and the following disclaimer,
 ** must be included in all copies of the Software, in whole or in part, and
 ** all derivative works of the Software, unless such copies or derivative
 ** works are solely in the form of machine-executable object code generated by
 ** a source language processor.
 ** 
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 ** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 ** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 ** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ** DEALINGS IN THE SOFTWARE.
 ** -LICENSE-END-
 */
//
//  BMDOpenGLOutput.h
//  OpenGLOutput
//

#ifndef __BMDOpenGLOutput_h__
#define __BMDOpenGLOutput_h__

#include <QtOpenGL>
#include <GL/glu.h>
#include "GLExtensions.h"

#include "DeckLinkAPI.h"
#include "GLScene.h"

class RenderDelegate;

class BMDOpenGLOutput
{
private:
	RenderDelegate*		pRenderDelegate;
	QGLWidget*			pContext;
	QMutex				Mutex;
	GLScene*			pGLScene;
	GLenum				glStatus;
	GLuint				idFrameBuf, idColorBuf, idDepthBuf;
	char*				pFrameBuf;

	// DeckLink
	uint32_t					uiFrameWidth;
	uint32_t					uiFrameHeight;
	
	IDeckLink*					pDL;
	IDeckLinkOutput*			pDLOutput;
	IDeckLinkMutableVideoFrame*	pDLVideoFrame;
	
	BMDTimeValue				frameDuration;
	BMDTimeScale				frameTimescale;
	uint32_t					uiFPS;
	uint32_t					uiTotalFrames;

	void ResetFrame();
	void SetPreroll();

public:
        BMDOpenGLOutput();
        ~BMDOpenGLOutput();

        bool InitDeckLink();
        bool InitGUI(IDeckLinkScreenPreviewCallback *previewCallback);
        bool InitOpenGL();
        uint32_t GetFPS();

        bool Start();
        void UpdateScene();
        bool Stop();

        void RenderToDevice();
};

////////////////////////////////////////////
// Render Delegate Class
////////////////////////////////////////////

class RenderDelegate : public IDeckLinkVideoOutputCallback
{
        BMDOpenGLOutput*	m_pOwner;

public:
        RenderDelegate (BMDOpenGLOutput* pOwner);
        ~RenderDelegate ();

        // IUnknown needs only a dummy implementation
        virtual HRESULT	STDMETHODCALLTYPE	QueryInterface (REFIID /*iid*/, LPVOID* /*ppv*/)	{return E_NOINTERFACE;}
        virtual ULONG	STDMETHODCALLTYPE	AddRef ()									{return 1;}
        virtual ULONG	STDMETHODCALLTYPE	Release ()									{return 1;}

        virtual HRESULT	STDMETHODCALLTYPE	ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
        virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped ();
};

#endif      // __BMDOpenGLOutput_h__
