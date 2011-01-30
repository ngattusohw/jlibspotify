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

#include "JAlbum.h"
#include "JUtils.h"

#include "Spotify/Spotify_Album.h"

Spotify::JAlbum* GetAlbumNativePtr( JNIEnv* env, jobject object )
{
	jclass cls = env->FindClass("Spotify/Album");
	jfieldID fid = env->GetFieldID( cls, "m_nativePtr", "I" );
	int nativePtr = env->GetIntField( object, fid );
	
	Spotify::JAlbum* pAlbum = reinterpret_cast<Spotify::JAlbum*>( Spotify::NativePtrToPointer( nativePtr ) );

	return pAlbum;
}

JNIEXPORT jstring JNICALL Java_Spotify_Album_GetName
  (JNIEnv *env, jobject object)
{
	Spotify::JAlbum* pAlbum = GetAlbumNativePtr( env, object );

	const char* name = pAlbum->GetName();
	jstring jstr = env->NewStringUTF( name );

	return jstr;
}

JNIEXPORT jboolean JNICALL Java_Spotify_Album_IsLoading
  (JNIEnv *env, jobject object)
{
	Spotify::JAlbum* pAlbum = GetAlbumNativePtr( env, object );

	return pAlbum->IsLoading();
}

JNIEXPORT void JNICALL Java_Spotify_Album_Release
  (JNIEnv *env, jobject object)
{
	Spotify::JAlbum* pAlbum = GetAlbumNativePtr( env, object );

	delete pAlbum;

	jclass cls = env->FindClass("Spotify/Album");
	jfieldID fid = env->GetFieldID( cls, "m_nativePtr", "I" );
	env->SetIntField( object, fid, 0 );
}