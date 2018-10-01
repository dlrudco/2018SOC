app.controller('PlayerController', ['$scope', function($scope){
	$scope.manifestUrl = "/None";

	window.addEventListener('load', startup);

	function startup(){
		initApp();
	}

	$scope.availableMpds = [
		{id: '/None', name: "None"},
		{id: '/vod/full_output_1280x720_2000k_dash.mpd', name: "Video"},
		{id: '/live/live_dash.mpd', name: "Live"},

	];

	$scope.videoStatus = {
	};

	setInterval(updateStatus, 1000);


	function updateStatus(){
		if(window.player == undefined)
			return;

		var videoStats = window.player.getStats();
		var fixval = 3;


		if(videoStats.playTime <= (window.player.f.duration - 1)){
			$scope.videoStatus.sessionTime = (videoStats.bufferingTime + videoStats.playTime).toFixed(fixval) + " sec";
			$scope.videoStatus.joinTime = (videoStats.loadLatency).toFixed(fixval) + " sec";
			$scope.videoStatus.bufferingTime = (videoStats.bufferingTime).toFixed(fixval) + " sec";
			$scope.videoStatus.playTime = (videoStats.playTime).toFixed(fixval) + " sec";		
			$scope.videoStatus.bufferingRatio = ((videoStats.bufferingTime) / (videoStats.bufferingTime + videoStats.playTime) * 100).toFixed(fixval) + " %";

			$scope.videoStatus.bitrate = (videoStats.streamBandwidth / 1024 / 1024).toFixed(fixval) + " Mbps";
			$scope.videoStatus.bandwidth = (videoStats.estimatedBandwidth / 1024 / 1024).toFixed(fixval) + " Mbps";

			$scope.videoInfo.readyState = window.player.K.readyState;
			$scope.videoInfo.mpd = $scope.manifestUrl;

			var curTime = window.player.f.currentTime;
			var min = Math.floor(curTime / 60);
			var sec = Math.floor(curTime % 60);

			$scope.videoInfo.curTime = min + ":" + sec;
			//console.log($scope.videoInfo.readyState);

			$scope.$digest();
		}
	}

	function initApp(){
		shaka.polyfill.installAll();
		if(shaka.Player.isBrowserSupported()){
			var video = document.getElementById('video');
			var player = new shaka.Player(video);
			window.player = player;
			player.addEventListener('error', onErrorEvent);
		}
		else{
			console.error('Browser not supported!');
		}
	}

	function initPlayer(){
		//New	
		var video = document.getElementById('video');
		var player = new shaka.Player(video);
		window.player = player;
		player.addEventListener('error', onErrorEvent);
	}

	function onErrorEvent(event){
		var err = event.detail;
		onError(event.detail);
	}

	function onError(error) {
		console.error('Error code', error.code, 'object', error);
	}

	function startView(){
		window.player.unload().then(function(){
			window.player.load("http://127.0.0.1:55555" + $scope.manifestUrl).then(function() {
				console.log(window.player.getConfiguration());
				console.log('The video has now been loaded!');

				$scope.videoInfo = {};
				$scope.$digest();

			}).catch(onError);
		});
	}

	$scope.onChangeMPD = function(){
		console.log("onChangeMPD : " + $scope.manifestUrl);
		startView();
		//initApp();
	}

}]);
