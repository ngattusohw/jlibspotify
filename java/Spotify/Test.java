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

package Spotify;

public class Test {
			
	public static void main(String[] argv)
	{
		System.out.println("Main - create session");
		Session session = new Session();
		System.out.println("Main - session created");
		
		Config config = new Config();
		
		config.m_appKey = AppData.g_appKey;
		config.m_appKeySize = AppData.g_appKey.length;
		config.m_cacheLocation = AppData.g_cacheLocation;
		config.m_settingsLocation = AppData.g_settingsLocation;
		config.m_tinySettings = false;
		config.m_userAgent = AppData.g_userAgent;
		
		System.out.println("Main - session initialising");
		int error = session.Initialise(config);
		System.out.printf("Main - session initialised [%d]\n", error);
		
		System.out.println("Main - logging in");
		session.Login( AppData.g_userName, AppData.g_password );
		while ( !session.IsLoggedIn() )
		{
			session.Update();
		}		
			
		System.out.println("Main - Get Playlist Container");
		Spotify.PlayListContainer pPlayLists = session.GetPlayListContainer();
		
		// LibSpotify can sometimes lose a playlist or two,
		//  so it's not a good idea to stall recursively on IsLoading()
		while (pPlayLists.IsLoading(false))
		{
			session.Update();
		}
		
		System.out.println("Main - Got Playlist Container");
		
		System.out.println("Main - dump TTY");
		pPlayLists.DumpToTTY();
		System.out.println("Main - dumped TTY");				
		
		System.out.println("** PLAY TRACK");

		Spotify.PlayListElement pElement = pPlayLists;
		while (pElement.HasChildren())
		{
			pElement = pElement.GetChild(0);
		}
		
		if (pElement.GetType() == Spotify.PlayListElement.TRACK)
		{
			pElement.InternalDumpToTTY(0);
			
			Spotify.Track pTrack = (Spotify.Track) pElement;
			
			int duration = pTrack.GetDuration();
			
			System.out.printf("Track Duration [%d]\n", duration );
						
			session.Load( pTrack );
			
			session.Play();

			while (true)
			{
				session.Update();
			}
		}
		
		//
		
		System.out.printf("Destroy playlist\n");
		pPlayLists.Release();
		pPlayLists = null;
		
		System.out.println("Main - logging out");
		session.Logout();
		while ( session.IsLoggedIn() )
		{
			session.Update();
		}		
		
		
		System.out.println("Main - session shutting down");
		session.Shutdown();
		System.out.println("Main - session shutdown");
		
		session = null;
	
		
	}
}
