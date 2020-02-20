package fi.joniaromaa.adobeair.discordrpc 
{
	import flash.events.Event;

	public class DiscordRpcEvent extends Event
	{
		public static var JOIN_REQUEST:String = "joinRequest";
		public static var JOIN_GAME:String = "joinGame";
		
		public function DiscordRpcEvent(tpye:String)
		{
			super(type);
		}
	}
}