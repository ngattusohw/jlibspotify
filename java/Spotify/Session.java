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

public class Session {

	static {
		// load the native Dynamic Library
		System.loadLibrary( "libspotify" );
		System.loadLibrary( "JLibSpotify" );
	}

	public int Initialise(Config config) {
		m_nativePtr = NativeCreate();

		return Initialise(m_nativePtr, config);
	}

	public void Shutdown() {
		Shutdown(m_nativePtr);

		NativeDestroy(m_nativePtr);

		m_nativePtr = 0;
	}

	public void Update() {
		Update(m_nativePtr);
	}

	public int Login(String username, String password) {
		return Login(m_nativePtr, username, password);
	}

	public int Logout() {
		return Logout(m_nativePtr);
	}

	public boolean IsLoggedIn() {
		return IsLoggedIn(m_nativePtr);
	}

	final int SP_CONNECTION_STATE_LOGGED_OUT = 0; // /< User not yet logged in
	final int SP_CONNECTION_STATE_LOGGED_IN = 1; // /< Logged in against a
													// Spotify access point
	final int SP_CONNECTION_STATE_DISCONNECTED = 2; // /< Was logged in, but has
													// now been disconnected
	final int SP_CONNECTION_STATE_UNDEFINED = 3; // /< The connection state is
													// undefined

	public int GetConnectionState() {
		return GetConnectionState(m_nativePtr);
	}

	public int Load(Track pTrack) {
		return Load(m_nativePtr, pTrack);
	}

	public void Unload(Track pTrack) {
		Unload(m_nativePtr, pTrack);
	}

	public Track GetCurrentTrack() {
		return GetCurrentTrack(m_nativePtr);
	}

	public int Seek(int offset) {
		return Seek(m_nativePtr, offset);
	}

	public int Play() {
		return Play(m_nativePtr);
	}

	public int Stop() {
		return Stop(m_nativePtr);
	}

	public int PreFetch(Track pTrack) {
		return PreFetch(m_nativePtr, pTrack);
	}

	public PlayListContainer GetPlayListContainer() {
		return GetPlayListContainer(m_nativePtr);
	}

	final int SP_BITRATE_160k = 0;
	final int SP_BITRATE_320k = 1;

	public void SetPreferredBitrate(int bitrate) {
		SetPreferredBitrate(m_nativePtr, bitrate);
	}

	public void OnLoggedIn(int error) {
		System.out.printf("JAVA - OnLoggedIn - error[%d]\n", error);
	}

	public void OnLoggedOut() {
		System.out.printf("JAVA - OnLoggedOut\n");
	}

	public void OnMetadataUpdated() {
		System.out.printf("JAVA - OnMetadataUpdated\n");
	}

	public void OnConnectionError(int error) {
		System.out.printf(" JAVA - OnConnectionError [%d]\n", error);
	}

	public void OnMessageToUser(String message) {
		System.out.printf("JAVA - OnMessageToUser [%s]", message);
	}

	public void OnNotifyMainThread() {
		System.out.println("JAVA - OnNotifyMainThread");
	}

	final int SP_SAMPLETYPE_INT16_NATIVE_ENDIAN = 0; // /< 16-bit signed integer
														// samples

	public int OnMusicDelivery(AudioFormat format, byte[] frames, int numFrames) {
		System.out.printf("OnMusicDelivery [%d]\n", numFrames);
		return numFrames;
	}

	public void OnPlayTokenLost() {
		System.out.printf("Java - OnPlayTokenLost\n");
	}

	public void OnLogMessage(String message) {
		System.out.printf("Java - OnLogMessage [%s]\n", message);
	}

	public void OnEndOfTrack() {
		System.out.printf("Java - OnEndOfTrack\n");
	}

	public void OnStreamingError(int error) {
	}

	public void OnUserinfoUpdated() {

	}

	public void OnStartPlayback() {
	}

	public void OnStopPlayback() {
	}

	public void OnGetAudioBufferStats(AudioBufferStats stats) {

	}

	private native int NativeCreate();

	private native void NativeDestroy(int nativePtr);

	private native int Initialise(int nativePtr, Config config);

	private native void Shutdown(int nativePtr);

	private native void Update(int nativePtr);

	private native int Login(int nativePtr, String username, String password);

	private native int Logout(int nativePtr);

	private native boolean IsLoggedIn(int nativePtr);

	private native int GetConnectionState(int nativePtr);

	private native int Load(int nativePtr, Track pTrack);

	private native void Unload(int nativePtr, Track pTrack);

	private native Track GetCurrentTrack(int nativePtr);

	private native int Seek(int nativePtr, int offset);

	private native int Play(int nativePtr);

	private native int Stop(int nativePtr);

	private native int PreFetch(int nativePtr, Track pTrack);

	private native PlayListContainer GetPlayListContainer(int nativePtr);

	private native void SetPreferredBitrate(int nativePtr, int bitrate);

	private int m_nativePtr = 0;
}
