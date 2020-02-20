package fi.joniaromaa.adobeair.discordrpc
{
	public class DiscordRpcJoinGameEvent extends DiscordRpcEvent
	{
		private var _secret:String;
		
		public function DiscordRpcJoinGameEvent(secret:String)
		{
			super(DiscordRpcEvent.JOIN_GAME);
			
			this._secret = secret;
		}
		
		public function get userId() : String
		{
			return this._secret;
		}
	}
}