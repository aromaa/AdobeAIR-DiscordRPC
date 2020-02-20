package fi.joniaromaa.adobeair.discordrpc
{
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	
	public class DiscordRpc extends EventDispatcher
	{
		private var extension:ExtensionContext;
		
		public var initialized:Boolean;
		
		public function DiscordRpc() 
		{
			this.initExtension();
		}
		
		public function initExtension() : void
		{
			this.extension = ExtensionContext.createExtensionContext("fi.joniaromaa.adobeair.discordrpc", null);
			if (this.extension != null)
			{
				this.initialized = true;
			}
		}
		
		private function statusEvent(event:StatusEvent) : void
		{
			return; //Figure it out later!
			
			if (event.code == "joinGame")
			{
				this.dispatchEvent(new DiscordRpcJoinGameEvent(this.popJoinSecret()));
			}
			else if (event.code == "joinRequest")
			{
				this.dispatchEvent(new DiscordRpcJoinRequestEvent(this.popJoinRequest()));
			}
		}
		
		public function get isSupported() : Boolean
		{
			return this.initialized;
		}
		
		public function init(applicationId:String) : void
		{
			if (!this.isSupported)
			{
				return; //Throw error
			}
			
			if (applicationId == null)
			{
				return; //Throw error
			}
			
			this.extension.call("init", applicationId);
			this.extension.addEventListener(StatusEvent.STATUS, this.statusEvent, false, 0, true);	
		}
		
		public function updatePresence(state:String, details:String, startTime:uint, endTime:uint, largeImage:String, largeImageDesc:String, smallImage:String, smallImageDesc:String, partyId:String, partySize:int, partyMax:int, joinSecret:String, specSecret:String) : void
		{
			if (!this.isSupported)
			{
				return; //Throw error
			}
			
			if (state == null)
			{
				state = "";
			}
			
			if (details == null)
			{
				details = "";
			}
			
			if (largeImage == null)
			{
				largeImage = "";
			}
			
			if (largeImageDesc == null)
			{
				largeImageDesc = "";
			}
			
			if (smallImage == null)
			{
				smallImage = "";
			}
			
			if (smallImageDesc == null)
			{
				smallImageDesc = "";
			}
			
			if (partyId == null)
			{
				partyId = "";
			}
			
			if (joinSecret == null)
			{
				joinSecret = "";
			}
			
			if (specSecret == null)
			{
				specSecret = "";
			}
			
			this.extension.call("updatePresence", state, details, startTime, endTime, largeImage, largeImageDesc, smallImage, smallImageDesc, partyId, partySize, partyMax, joinSecret, specSecret);
		}
		
		public function runCallbacks() : void
		{
			if (!this.isSupported)
			{
				return; //Throw error
			}
			
			this.extension.call("runCallbacks");
		}
		
		public function respond(userId:String, reply:int) : void
		{
			if (!this.isSupported)
			{
				return; //Throw error
			}
			
			if (userId == null)
			{
				return; //Throw error
			}
			
			this.extension.call("respond", userId, reply);
		}
		
		//Internal ones
		private function popJoinSecret() : String
		{
			return this.extension.call("popJoinSecret") as String;
		}
		
		private function popJoinRequest() : String
		{
			return this.extension.call("popJoinRequest") as String;
		}
	}
}