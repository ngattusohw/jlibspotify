/*
 * Copyright 2011 Jim Knowler
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#pragma once

#include <jni.h>

#include "Spotify/Session.h"

namespace Spotify
{
	
	class JSession : public Spotify::Session
	{
	public:
		JSession(JNIEnv *env, jobject object);
		virtual ~JSession();

		// factory functions
		virtual PlayList*			CreatePlayList();
		virtual PlayListContainer*	CreatePlayListContainer();
		virtual PlayListFolder*		CreatePlayListFolder();
		virtual Track*				CreateTrack();
		virtual Artist*				CreateArtist();
		virtual Album*				CreateAlbum();
		virtual Image*				CreateImage();

		JNIEnv* GetEnv();

	protected:
		
		virtual void OnLoggedIn( sp_error error );
		virtual void OnLoggedOut();
		virtual void OnMetadataUpdated();
		virtual void OnConnectionError( sp_error error );
		virtual void OnMessageToUser( const char* message );
		virtual void OnNotifyMainThread();
		virtual int  OnMusicDelivery( const sp_audioformat* format, const void* frames, int num_frames );
		virtual void OnPlayTokenLost();
		virtual void OnLogMessage( const char* data );
		virtual void OnEndOfTrack();
		virtual void OnStreamingError( sp_error error );
		virtual void OnUserinfoUpdated();
		virtual void OnStartPlayback();
		virtual void OnStopPlayback();
		virtual void OnGetAudioBufferStats( sp_audio_buffer_stats* stats );
	private:

		void		JNICallVoidMethod( const char* name, const char* sig, ... );

		JNIEnv*		m_env;
		jobject		m_session;
	};
}
