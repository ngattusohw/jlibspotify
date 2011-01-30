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

#include "JArtist.h"
#include "JUtils.h"

#include "Spotify/Spotify_Artist.h"

static Spotify::JArtist* GetArtistNativePtr( JNIEnv* env, jobject object )
{
	jclass cls = env->FindClass("Spotify/Artist");
	jfieldID fid = env->GetFieldID( cls, "m_nativePtr", "I" );
	int nativePtr = env->GetIntField( object, fid );
	
	Spotify::JArtist* pArtist = reinterpret_cast<Spotify::JArtist*>( Spotify::NativePtrToPointer( nativePtr ) );

	return pArtist;
}


JNIEXPORT jstring JNICALL Java_Spotify_Artist_GetName
  (JNIEnv *env, jobject object)
{
	Spotify::JArtist* pArtist = GetArtistNativePtr( env, object );

	const char* name = pArtist->GetName();
	jstring jstr = env->NewStringUTF( name );

	return jstr;
}

JNIEXPORT jboolean JNICALL Java_Spotify_Artist_IsLoading
  (JNIEnv *env, jobject object)
{
	Spotify::JArtist* pArtist = GetArtistNativePtr( env, object );

	return pArtist->IsLoading();
}

JNIEXPORT void JNICALL Java_Spotify_Artist_Release
  (JNIEnv *env, jobject object)
{
	Spotify::JArtist* pArtist = GetArtistNativePtr( env, object );

	delete pArtist;

	jclass cls = env->FindClass("Spotify/Artist");
	jfieldID fid = env->GetFieldID( cls, "m_nativePtr", "I" );
	env->SetIntField( object, fid, 0 );
}
