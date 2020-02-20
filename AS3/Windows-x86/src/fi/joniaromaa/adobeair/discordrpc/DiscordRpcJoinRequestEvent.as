package fi.joniaromaa.adobeair.discordrpc
{
	public class DiscordRpcJoinRequestEvent extends DiscordRpcEvent
	{
		private var _userId:String;
		
		public function DiscordRpcJoinRequestEvent(userId:String)
		{
			super(DiscordRpcEvent.JOIN_REQUEST);
			
			this._userId = userId;
		}
		
		public function get userId() : String
		{
			return this._userId;
		}
	}
}