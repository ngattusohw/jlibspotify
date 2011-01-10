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

#include "JTrack.h"
#include "JSession.h"

// JNI
#include "Spotify/Spotify_Track.h"

JNIEXPORT jint JNICALL Java_Spotify_Track_GetDuration
  (JNIEnv *env, jobject object)
{
	Spotify::PlayListElement* pElement = Spotify::JPlayListElement::GetPlayListElement( env, object );
	Spotify::Track* pTrack = static_cast<Spotify::Track*>( pElement );
	
	return pTrack->GetDuration();
}

namespace Spotify
{

	JTrack::JTrack( JSession* pSession ) : Track( pSession )
	{
		InitialiseJavaObject( pSession->GetEnv(), this, "Spotify/Track" );
	}

}