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

#include "JSession.h"
#include "JTrack.h"
#include "JPlayList.h"
#include "JPlayListContainer.h"
#include "JPlayListFolder.h"
#include "JArtist.h"
#include "JAlbum.h"

#include "JUtils.h"

#include "Spotify/Spotify_Session.h"
#include "Spotify/Spotify_Session_Config.h"

#define LOG(msg,...)	//

JNIEXPORT jint JNICALL Java_Spotify_Session_NativeCreate
  (JNIEnv *env, jobject object)
{
	Spotify::JSession* pSession = new Spotify::JSession( env, object );
		
	return Spotify::PointerToNativePtr( pSession );
}

JNIEXPORT void JNICALL Java_Spotify_Session_NativeDestroy
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	delete pSession;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Initialise
  (JNIEnv *env, jobject object, jint nativePtr, jobject jconfig)
{
	jboolean isCopy;
	jfieldID fid;

	jclass cls = env->GetObjectClass(jconfig);
	
	// appkey
	fid = env->GetFieldID( cls, "m_appKey", "[C" );
	jobject appKeyObj = env->GetObjectField( jconfig, fid );
	jcharArray appKeyCharArray = static_cast<jcharArray>(appKeyObj);
	jchar* appKey = env->GetCharArrayElements( appKeyCharArray, &isCopy );

	fid = env->GetFieldID( cls, "m_appKeySize", "I" );
	jint appKeySize = env->GetIntField( jconfig, fid );

	// make local binary copy of key
	uint8_t* localAppKey = new uint8_t[ appKeySize ];
	for (int i=0; i<appKeySize; i++)
	{
		localAppKey[i] = (uint8_t) appKey[i];
	}

	fid = env->GetFieldID( cls, "m_cacheLocation", "Ljava/lang/String;" );	
	jstring strCacheLocation = static_cast<jstring>( env->GetObjectField( jconfig, fid ) );
	const char* szCacheLocation = env->GetStringUTFChars(strCacheLocation, &isCopy );

	fid = env->GetFieldID( cls, "m_settingsLocation", "Ljava/lang/String;" );	
	jstring strSettingsLocation = static_cast<jstring>( env->GetObjectField( jconfig, fid ) );
	const char* szSettingsLocation = env->GetStringUTFChars(strSettingsLocation, &isCopy );

	fid = env->GetFieldID( cls, "m_tinySettings", "Z" );
	jboolean tinySettings = env->GetBooleanField( jconfig, fid );

	fid = env->GetFieldID( cls, "m_userAgent", "Ljava/lang/String;" );	
	jstring strUserAgent = static_cast<jstring>( env->GetObjectField( jconfig, fid ) );
	const char* szUserAgent = env->GetStringUTFChars(strUserAgent, &isCopy );
  	
	Spotify::JSession::Config config;
	config.m_appKey = localAppKey;
	config.m_appKeySize = appKeySize;
	config.m_cacheLocation = szCacheLocation;
	config.m_settingsLocation = szSettingsLocation;
	config.m_tinySettings = (tinySettings != 0);
	config.m_userAgent = szUserAgent;

	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	sp_error error = pSession->Initialise( config );
	
	env->ReleaseStringUTFChars(strCacheLocation, szCacheLocation);
	env->ReleaseStringUTFChars(strSettingsLocation, szSettingsLocation);
	env->ReleaseStringUTFChars(strUserAgent, szUserAgent);

	delete [] localAppKey;
	env->ReleaseCharArrayElements( appKeyCharArray, appKey, 0 );
	
	return error;
}

JNIEXPORT void JNICALL Java_Spotify_Session_Shutdown
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	pSession->Shutdown();		
}

JNIEXPORT void JNICALL Java_Spotify_Session_Update
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	pSession->Update();
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Login
  (JNIEnv *env, jobject object, jint nativePtr, jstring username, jstring password)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	jboolean isCopy;
	const char* szUsername = env->GetStringUTFChars(username, &isCopy );
	const char* szPassword = env->GetStringUTFChars(password, &isCopy );

	sp_error error = pSession->Login( szUsername, szPassword );

	env->ReleaseStringUTFChars( username, szUsername );
	env->ReleaseStringUTFChars( password, szPassword );

	return error;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Logout
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	sp_error error = pSession->Logout();
	return error;
}

JNIEXPORT jboolean JNICALL Java_Spotify_Session_IsLoggedIn
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	return pSession->IsLoggedIn();
}

JNIEXPORT jint JNICALL Java_Spotify_Session_GetConnectionState
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	return pSession->GetConnectionState();
}

static Spotify::Track* GetTrackFromNativePtr( JNIEnv* env, jobject track )
{
	// get Track ptr from track->m_nativePtr
	jclass cls = env->FindClass("Spotify/Track");
	jfieldID fid = env->GetFieldID( cls, "m_nativePtr", "I");
	
	jint nativePtr = env->GetIntField( track, fid );
	Spotify::PlayListElement* pElement = reinterpret_cast< Spotify::PlayListElement* >( Spotify::NativePtrToPointer( nativePtr ) );

	return static_cast< Spotify::Track* >( pElement );
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Load
  (JNIEnv *env, jobject object, jint nativePtr, jobject track)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	Spotify::Track* pTrack = GetTrackFromNativePtr( env, track );

	return pSession->Load( pTrack );
}

JNIEXPORT void JNICALL Java_Spotify_Session_Unload
  (JNIEnv *env, jobject object, jint nativePtr, jobject track)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	Spotify::Track* pTrack = GetTrackFromNativePtr( env, track );
	
	pSession->Unload( pTrack );
}

JNIEXPORT jobject JNICALL Java_Spotify_Session_GetCurrentTrack
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	Spotify::Track* pTrack = pSession->GetCurrentTrack();

	if (pTrack)
	{
		Spotify::JTrack* pJTrack = static_cast<Spotify::JTrack*>( pTrack );
		jobject jtrack = pJTrack->GetJavaObject();
		
		return jtrack;
	}

	return NULL;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Seek
  (JNIEnv *env, jobject object, jint nativePtr, jint offset)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	sp_error error = pSession->Seek( offset );
	return error;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Play
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	sp_error error = pSession->Play();
	return error;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_Stop
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	sp_error error = pSession->Stop();
	return error;
}

JNIEXPORT jint JNICALL Java_Spotify_Session_PreFetch
  (JNIEnv *env, jobject object, jint nativePtr, jobject track)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	Spotify::Track* pTrack = GetTrackFromNativePtr( env, track );

	sp_error error = pSession->PreFetch( pTrack );
	return error;
}

JNIEXPORT jobject JNICALL Java_Spotify_Session_GetPlayListContainer
  (JNIEnv *env, jobject object, jint nativePtr)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );

	Spotify::JPlayListContainer* pPlayListContainer = static_cast<Spotify::JPlayListContainer*>( pSession->GetPlayListContainer() );

	if (pPlayListContainer)
	{
		jobject jPlayListContainer = pPlayListContainer->GetJavaObject();

		return jPlayListContainer;
	}
	else
	{
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_Spotify_Session_SetPreferredBitrate
  (JNIEnv *env, jobject object, jint nativePtr, jint bitrate)
{
	Spotify::JSession* pSession = reinterpret_cast< Spotify::JSession* >( Spotify::NativePtrToPointer( nativePtr ) );
	pSession->SetPreferredBitrate( static_cast<sp_bitrate>(bitrate) );
}

namespace Spotify
{
	JSession::JSession(JNIEnv *env, jobject session)
	{
		m_env = env;
		m_session = m_env->NewGlobalRef( session );
	}

	JSession::~JSession()
	{
		m_env->DeleteGlobalRef( m_session );
		m_session = NULL;
	}

	PlayList* JSession::CreatePlayList()
	{		
		return new JPlayList( this );
	}

	PlayListContainer* JSession::CreatePlayListContainer()
	{
		return new JPlayListContainer( this );
	}

	PlayListFolder* JSession::CreatePlayListFolder()
	{
		return new JPlayListFolder( this );
	}

	Track* JSession::CreateTrack()
	{
		return new JTrack( this );
	}

	Artist* JSession::CreateArtist()
	{
		return new JArtist();
	}

	Album* JSession::CreateAlbum()
	{
		return new JAlbum();
	}

	void JSession::JNICallVoidMethod( const char* name, const char* sig, ... )
	{
		JNIEnv* env = NULL;
		JavaVM* vm = NULL;
		
		m_env->GetJavaVM( &vm );		
		vm->AttachCurrentThread( (void**) &env, NULL );

		jclass cls = env->GetObjectClass(m_session);

		if (cls != 0)
		{		
			jmethodID mid = env->GetMethodID( cls, name, sig );
			if (mid != 0)
			{		
				va_list args;
				va_start (args, sig);		
				env->CallVoidMethodV( m_session, mid, args );		
				va_end(args);	
			}
		}
		
	}

	void JSession::OnLoggedIn( sp_error error )
	{
		Session::OnLoggedIn( error );
		LOG("OnLoggedIn");

		JNICallVoidMethod( "OnLoggedIn", "(I)V", jint(error) );			
	}

	void JSession::OnLoggedOut()
	{
		Session::OnLoggedOut();
		LOG("OnLoggedOut");

		JNICallVoidMethod( "OnLoggedOut", "()V" );
	}

	void JSession::OnMetadataUpdated()
	{
		Session::OnMetadataUpdated();
		LOG("OnMetadataUpdated");

		JNICallVoidMethod( "OnMetadataUpdated", "()V" );
	}

	void JSession::OnConnectionError( sp_error error )
	{
		Session::OnConnectionError( error );
		LOG("OnConnectionError");

		JNICallVoidMethod( "OnConnectionError", "(I)V", jint(error) );
	}

	void JSession::OnMessageToUser( const char* message )
	{
		Session::OnMessageToUser( message );

		LOG("OnMessageToUser");
		LOG( message );

		jstring jstr = m_env->NewStringUTF( message );
		JNICallVoidMethod( "OnLoggedOut", "(Ljava/lang/String;)V", jstr );		
	}

	void JSession::OnNotifyMainThread()
	{
		Session::OnNotifyMainThread();
		LOG("OnNotifyMainThread");
		
		JNICallVoidMethod( "OnNotifyMainThread", "()V" );
	}

	int  JSession::OnMusicDelivery( const sp_audioformat* format, const void* frames, int num_frames )
	{
		LOG("OnMusicDelivery");

		JNIEnv* env = NULL;
		JavaVM* vm = NULL;
		
		// get env for current thread
		m_env->GetJavaVM( &vm );		
		vm->AttachCurrentThread( (void**) &env, NULL );		

		// prepare sample data
		int sampleSize = 2 * format->channels;
		int numBytes = num_frames * sampleSize;

		jbyteArray byteArray = env->NewByteArray( numBytes );
		
		jboolean isCopy = false;
		jbyte* pByteData = env->GetByteArrayElements( byteArray, &isCopy );

		for (int i=0; i<numBytes; i++)
		{
			pByteData[i] = ( (uint8_t*) frames)[i];
		}
		
		env->ReleaseByteArrayElements( byteArray, pByteData, 0);
		pByteData = 0;

		// prepare audioFormat
		jclass clsa = env->FindClass("Spotify/AudioFormat");
		jmethodID cida = env->GetMethodID(clsa, "<init>", "()V");
		jobject jaudioFormat = env->NewObject( clsa, cida );

		jfieldID sampleTypeID = env->GetFieldID( clsa, "m_sampleType", "I");
		jfieldID sampleRateID = env->GetFieldID( clsa, "m_sampleRate", "I");
		jfieldID channelsID = env->GetFieldID( clsa, "m_channels", "I");
		env->SetIntField( jaudioFormat, sampleTypeID, format->sample_type );
		env->SetIntField( jaudioFormat, sampleRateID, format->sample_rate );
		env->SetIntField( jaudioFormat, channelsID, format->channels );

		// make the callback
		jclass cls = env->GetObjectClass(m_session);				
		jmethodID mid = env->GetMethodID( cls, "OnMusicDelivery", "(LSpotify/AudioFormat;[BI)I" );		

		int retValue = env->CallIntMethod( m_session, mid, jaudioFormat, byteArray, num_frames );		

		return retValue;		
	}

	void JSession::OnPlayTokenLost()
	{
		Session::OnPlayTokenLost();
		LOG("OnPlayTokenLost");

		JNICallVoidMethod( "OnPlayTokenLost", "()V" );
	}

	void JSession::OnLogMessage( const char* data )
	{
		Session::OnLogMessage( data );

		LOG("OnLogMessage");
		LOG( data );
		
		jstring jstr = m_env->NewStringUTF( data );		
		JNICallVoidMethod( "OnLogMessage", "(Ljava/lang/String;)V", jstr );		
		m_env->DeleteLocalRef( jstr );		
	}

	void JSession::OnEndOfTrack()
	{
		Session::OnEndOfTrack();

		LOG("OnEndOfTrack");

		JNICallVoidMethod( "OnEndOfTrack", "()V" );
	}

	void JSession::OnStreamingError( sp_error error )
	{
		Session::OnStreamingError( error );
		LOG("OnStreamingError");

		JNICallVoidMethod( "OnStreamingError", "(I)V" );
	}

	void JSession::OnUserinfoUpdated()
	{
		Session::OnUserinfoUpdated();
		LOG("OnUserinfoUpdated");

		JNICallVoidMethod( "OnUserinfoUpdated", "()V" );
	}

	void JSession::OnStartPlayback()
	{
		Session::OnStartPlayback();
		LOG("OnStartPlayback");

		JNICallVoidMethod( "OnStartPlayback", "()V" );
	}

	void JSession::OnStopPlayback()
	{
		Session::OnStopPlayback();
		LOG("OnStopPlayback");

		JNICallVoidMethod( "OnStopPlayback", "()V" );
	}

	void JSession::OnGetAudioBufferStats( sp_audio_buffer_stats* stats )
	{
		Session::OnGetAudioBufferStats( stats );
		LOG("OnGetAudioBufferStats");

		JNIEnv* env = NULL;
		JavaVM* vm = NULL;
		
		// get env for current thread
		m_env->GetJavaVM( &vm );		
		vm->AttachCurrentThread( (void**) &env, NULL );	

		jclass cls = env->FindClass("Spotify/AudioBufferStats");
		jmethodID cid = env->GetMethodID(cls, "<init>", "()V");
		jobject jstats = env->NewObject( cls, cid );

		jfieldID samplesID = env->GetFieldID( cls, "m_samples", "I");
		jfieldID stutterID = env->GetFieldID( cls, "m_stutter", "I");

		JNICallVoidMethod( "OnGetAudioBufferStats", "(LSpotify/AudioBufferStats;)V", jstats );

		stats->samples = env->GetIntField( jstats, samplesID );
		stats->stutter = env->GetIntField( jstats, stutterID );		
	}

	JNIEnv* JSession::GetEnv()
	{
		return m_env;
	}
}

