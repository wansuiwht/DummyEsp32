/**
 * @file builtinfiles.h
 * @brief This file is part of the WebServer example for the ESP8266WebServer.
 *
 * This file contains long, multiline text variables for  all builtin resources.
 */
static const char setWiFi[] PROGMEM = 
R"==(
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Wifi设置</title>
	</head>
	<body>
		<p>
			WIFI 设置
		</p>
		<p>
			<span>帐号:</span>
			<input id="ssid" type="text" name="ssid" value="wifi01"/>
		</p>
		<p>
			<span>密码:</span>
			<input id="pass" type ="text" name="password" value="Hysim8888"/>
		</p>
		<button id="submit">确定</button>
		<script>
			function submit(){
				var ssid = document.getElementById('ssid')
				var pass = document.getElementById("pass")
				console.log(ssid.value,pass.value);
				fetch('/setWifi?ssid=' + ssid.value + "&pass=" + pass.value,{method:'GET'}).then((res)=>{
					console.log(res)
					alert(res.msg)
				})
			}
			var btn = document.getElementById('submit');
			btn.addEventListener('click',submit,false)
			
		</script>
	</body>
</html>
)==";
// used for $upload.htm
static const char uploadContent[] PROGMEM =
  R"==(
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Web控制台 V0.1 pre-alpha</title>
		<link rel="stylesheet" href="https://unpkg.com/element-ui@2.15.14/lib/theme-chalk/index.css">
		<script src="https://aframe.io/releases/1.5.0/aframe.min.js"></script>
		<script src="https://unpkg.com/aframe-environment-component@1.3.7/dist/aframe-environment-component.min.js">
		</script>
		<style>
			.el-header,
			.el-footer {
				background-color: #333;
				color: #fff;
				line-height: 60px;
				font-size: 30px;
			}
			.el-main {
				background-color: #E9EEF3;
				color: #333;
				.el-col {
					border-radius: 4px;
				}
				.bg-purple-dark {
					background: #99a9bf;
				}
				.bg-purple {
					background: #d3dce6;
				}
				.bg-purple-light {
					background: #e5e9f2;
				}
				.grid-content {
					border-radius: 4px;
				}
				.text {
					font-size: 14px;
				}
				.item {
					margin-bottom: 18px;
				}
				
				.box-card {
					width: 100%;
					height: 150px;
				}
				.el-card__header{
					padding: 8px 20px;
				}
				.pidValue {
					max-width: 5.125rem;
				}
				.infoLabel{
					display: flex;
					justify-content: center;
				}
		</style>
	</head>
	<body style="background-color: #E9EEF3;">

		<div id="app">
			<el-container>
				<el-header>Web 控制台 V0.1 Pre-Alpha</el-header>
				<el-main>
					<el-row :gutter="6">
						<el-row :gutter="6">
							<el-col :span="12">
								<el-card class="box-card">
									<div slot="header" >
										<span>系统信息</span>
									</div>
									<el-row>
										<el-col :span="2">
											<div class="infoLabel">
												SSID
											</div>
										</el-col>
										<el-col :span="6">
											<el-input v-model="ssid" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
										<el-col :span="2">
										<div class="infoLabel">
											密码
										</div>
										</el-col>
										<el-col :span="6">
											<el-input v-model="wifiPassword" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
										<el-col :span="8">
											<el-button @click="resetWifi" type="primary" size="mini">重置WIFI</el-button>
										</el-col>
										<el-col :span="3">
										<div class="infoLabel">
											ROS Agent Ip
										</div>
										</el-col>
										<el-col :span="5">
											<el-input v-model="agentIp" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
										<el-col :span="4">
										<div class="infoLabel">
											ROS Agent Port
										</div>
										</el-col>
										<el-col :span="4">
											<el-input v-model="agentPort" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
										<el-col :span="8">
											<el-button @click="startRos" type="primary" size="mini">开启Ros节点</el-button>
										</el-col>
										<el-col :span="2">
											<div class="infoLabel">
												UDP端口
											</div>
										</el-col>
										<el-col :span="6">
											<el-input v-model="udpPort" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
										<el-col :span="2">
										<div class="infoLabel">
											TCP端口
										</div>
										</el-col>
										<el-col :span="6">
											<el-input v-model="tcpPort" placeholder="请输入内容" size="mini"></el-input>
										</el-col>
									</el-row>
								</el-card>
								<el-card class="box-card" style="margin-top: 6px; height: 110px">
									<div slot="header" >
										<span>命令</span>
									</div>
									<el-row style="margin-top: 5px;">
										<el-col :span="12">
											<el-select v-model="ctrlCmd" placeholder="请选择命令" size="mini">
												<el-option v-for="item in ctrlCmdOptions" :key="item.value"
													:label="item.label" :value="item.value">
												</el-option>
											</el-select>
											<el-button @click="cmdRun" type="primary" size="mini">发送</el-button>

										</el-col>
										<el-col :span="12">
											<span>命令模式</span>
											<el-select v-model="cmdMode" placeholder="请选择命令模式" size="mini">
												<el-option v-for="item in cmdModeOptions" :key="item.value"
													:label="item.label" :value="item.value">
												</el-option>
											</el-select>
											<el-button @click="onSetCmdMode" type="primary" size="mini">设置</el-button>
										</el-col>
									</el-row>
								</el-card>
								<el-card style="height: 280px;margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>运行</span>
										<el-input-number v-model="speed" :min="1" :max="150" size="mini"
											style="float: right; padding: 3px 0" :controls="false"></el-input-number>
										<span style="float: right; padding: 3px 3px">速度</span>
									</div>
									<el-row :gutter="10">
										<el-col :span="12">
											<el-row>
											<el-col :span="8">
												<span>J1</span>
												<el-slider v-model="J1Pos" :min="J1Min" :max="J1Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J1Pos" :min="J1Min" :max="J1Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>J2</span>
												<el-slider v-model="J2Pos" :min="J2Min" :max="J2Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J2Pos" :min="J2Min" :max="J2Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>J3</span>
												<el-slider v-model="J3Pos" :min="J3Min" :max="J3Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J3Pos" :min="J3Min" :max="J3Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											</el-row>
											<el-row>
											<el-col :span="8">
												<span>J4</span>
												<el-slider v-model="J4Pos" :min="J4Min" :max="J4Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J4Pos" :min="J4Min" :max="J4Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>J5</span>
												<el-slider v-model="J5Pos" :min="J5Min" :max="J5Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J5Pos" :min="J5Min" :max="J5Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>J6</span>
												<el-slider v-model="J6Pos" :min="J6Min" :max="J6Max" @change="onPosChange"
													@input="onInput"></el-slider>
												<el-input-number v-model="J6Pos" :min="J6Min" :max="J6Max" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											</el-row>
											<el-row style="margin-top: 15px;">
												<el-button @click="resetJoints" type="primary" size="mini">重置</el-button>
												<el-button @click="addToCommandList" type="primary"
													size="mini">添加到命令列表</el-button>
											</el-row>
										</el-col>
										<el-col :span="12">
											<el-row>
											<el-col :span="8">
												<span>X</span>
												<el-slider v-model="X" :min="XyzMin" :max="XyzMax"></el-slider>
												<el-input-number v-model="X"  size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>Y</span>
												<el-slider v-model="Y" :min="XyzMin" :max="XyzMax"></el-slider>
												<el-input-number v-model="Y"  size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>Z</span>
												<el-slider v-model="Z" :min="XyzMin" :max="XyzMax"></el-slider>
												<el-input-number v-model="Z"  size="mini"
													:controls="false"></el-input-number>
											</el-col>
											</el-row>
											<el-row>
											<el-col :span="8">
												<span>A</span>
												<el-slider v-model="A" :min="XyzMin" :max="XyzMax"></el-slider>
												<el-input-number v-model="A" size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>B</span>
												<el-slider v-model="B" :min="XyzMin" :max="XyzMax" ></el-slider>
												<el-input-number v-model="B"  size="mini"
													:controls="false"></el-input-number>
											</el-col>
											<el-col :span="8">
												<span>C</span>
												<el-slider v-model="C" :min="XyzMin" :max="XyzMax"></el-slider>
												<el-input-number v-model="C"  size="mini"
													:controls="false"></el-input-number>
											</el-col>
											</el-row>
											<el-row style="margin-top: 15px;">
												<el-button @click="refreshJointsL" type="primary" size="mini">刷新</el-button>
												<el-button @click="resetJointsL" type="primary" size="mini">重置</el-button>
												<el-button @click="sendXYZ" type="primary" size="mini">发送</el-button>
												<el-button @click="addXYZToCommandList" type="primary"
													size="mini">添加到命令列表</el-button>
											</el-row>
										</el-col>
									</el-row>
								
								</el-card>
								<el-card style="height:300px; margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>命令列表</span>
										<div style="float: right;">
											<el-radio v-model="cmdListRunMode" label="1">单次运行</el-radio>
											<el-radio v-model="cmdListRunMode" label="2">循环运行</el-radio>
											<el-button @click="runCmdList" type="primary" size="mini">{{cmdExecFlag}}</el-button>
											<el-button @click="clearCmdList" type="primary" size="mini">清除</el-button>
											<el-button @click="animat" type="primary" size="mini">导入命令</el-button>
										</div>
									</div>
									<ol>
										<li v-for="item,index in dummyCmdList" class="infinite-list-item" :style="index == cmdIndex -1 ? 'background: cornflowerblue;' : 'background:white;' "  >{{ item.cmd }}</li>
									</ol>
								</el-card>
								<el-card style="margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>关节电机</span>
									</div>
									<el-row :gutter="6">
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J1</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(1)" type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[0].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[0].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[0].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J2</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(2)" type="text">设置</el-button>
												</div>

												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[1].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[1].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[1].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J3</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(3)" type="text">设置</el-button>
												</div>

												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[2].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[2].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[2].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
									</el-row>
									<el-row :gutter="6" style="margin-top: 6px;">
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J4</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(4)" type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[3].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[3].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[3].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J5</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(5)" type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[4].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[4].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[4].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J6</span>
													<el-button style="float: right; padding: 3px 0"
														@click="onSetMotorPID(6)" type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="motorPid[5].kp"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="motorPid[5].ki"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="motorPid[5].kd"
															:controls="false" :min="-360"
															:max="pidMax"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
									</el-row>
								</el-card>
							</el-col>
							<el-col :span="12">
								<el-card style="min-height: 200px;padding: 0px; ">
									<div slot="header" class="clearfix">
										<span>仿真</span>
										<el-checkbox style="float: right; padding: 3px 0">录制</el-checkbox>
									</div>
									<div style="width: 100%;height: 820px;">
										<a-scene style="width: 100%;height: 100%;" embedded background="color: #212"
											environment>
											<a-assets>
												<a-asset-item id="base"
													src="http://qiniu.wansuiwht.top/Model/base.glb"></a-asset-item>
												<a-asset-item id="j1"
													src="http://qiniu.wansuiwht.top/Model/j1.glb"></a-asset-item>
												<a-asset-item id="j2"
													src="http://qiniu.wansuiwht.top/Model/j2j3.glb"></a-asset-item>
												<a-asset-item id="j4"
													src="http://qiniu.wansuiwht.top/Model/j4.glb"></a-asset-item>
												<a-asset-item id="j5"
													src="http://qiniu.wansuiwht.top/Model/j5.glb"></a-asset-item>
												<a-asset-item id="j6"
													src="http://qiniu.wansuiwht.top/Model/j6.glb"></a-asset-item>
											</a-assets>
											<a-entity position="0 1.6 0.00" camera
												look-controls="magicWindowTrackingEnabled: false; touchEnabled: false; mouseEnabled: true">
												<a-entity id="fadeBackground" geometry="primitive: sphere; radius: 2.5"
													material="color: black; side: back; shader: flat; transparent: true; opacity: 0.6"
													visible="false">
												</a-entity>
											</a-entity>
											<a-entity ref="roboot" gltf-model="#base" modify-materials
												position="0 1.4 -0.6" rotation="0 45 0">
												<a-entity ref="j1" gltf-model="#j1" modify-materials
													position="0 0.057 0.00" rotation="0 0 0">
													<a-entity ref="j2" gltf-model="#j2" modify-materials
														position="-0.017 0.033 0.035" rotation="0 -90 0">
														<a-entity ref="j3" gltf-model="#j4" modify-materials
															position="0 0.146 -0.033" rotation="180 0 0">
															<a-entity ref="j4" gltf-model="#j5" modify-materials
																position="0.015 -0.05 -0.02" rotation="0 90 0">
																<a-entity ref="j5" gltf-model="#j6" modify-materials
																	position="0.015 0 0.105" rotation="0 90 0">
																</a-entity>
															</a-entity>
														</a-entity>
													</a-entity>
												</a-entity>
											</a-entity>
										</a-scene>
									</div>
								</el-card>
								<el-card style="min-height: 150px; margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>日志</span>
									</div>
									<el-input type="textarea" v-model="dummyLog" :rows="8" readonly v-model="textarea">
									</el-input>
								</el-card>
							</el-col>
						</el-row>
					</el-row>

				</el-main>
			</el-container>

			<el-dialog :visible.sync="visible" title="Hello world">
				<p>Try Element</p>
			</el-dialog>
		</div>

	</body>
	<script src="https://unpkg.com/vue@2/dist/vue.js"></script>
	<script src="https://unpkg.com/axios/dist/axios.min.js"></script>
	<script src="https://unpkg.com/element-ui@2.15.14/lib/index.js"></script>
	<script setup type="module">
		new Vue({
					el: '#app',
					data: function() {
						return {
							visible: false,
							agentIp: "192.168.0.110",
							agentPort : 2018,
							dummyLog: "started ",
							dummyCmdList: [],
							cmdListRunMode: '1',
							ip: '0.0.0.0',
							ssid:'',
							wifiPassword:'',
							udpPort: 9999,
							tcpPort: 9999,
							speed: 10,
							J1Pos: 0,
							J1Min: 0,
							J1Max: 100,
							J2Pos: 0,
							J2Min: 0,
							J2Max: 100,
							J3Pos: 0,
							J3Min: 0,
							J3Max: 100,
							J4Pos: 0,
							J4Min: 0,
							J4Max: 100,
							J5Pos: 0,
							J5Min: 0,
							J5Max: 100,
							J6Pos: 0,
							J6Min: 0,
							J6Max: 100,
							XyzMin:-100000,
							XyzMax: 100000,
							X:0,Y:0,Z:0,A:0,B:0,C:0,
							ctrlCmd: '',
							cmdModeOptions: [{value:2,label:'可中断'},{value:3,label:"顺序执行"}],
							cmdMode: 3,
							cmdIndex: 0,
							pidMax: 5000,
							pidMin: 0,
							allowSendCmd: true,
							websocket: null,
							cmdExecFlag: '执行',
							motorPid: [{
									kp: 1,
									ki: 11,
									kd: 111
								},
								{
									kp: 2,
									ki: 22,
									kd: 222
								},
								{
									kp: 3,
									ki: 33,
									kd: 333
								},
								{
									kp: 4,
									ki: 44,
									kd: 444
								},
								{
									kp: 5,
									ki: 55,
									kd: 555
								},
								{
									kp: 6,
									ki: 66,
									kd: 666
								},
							],
							ctrlCmdOptions: [{
									value: '!HOME',
									label: 'HOME'
								}, {
									value: '!START',
									label: 'START'
								},
								{
									value: '!CALIBRATION',
									label: 'CALIBRATTION'
								},
								{
									value: '!RESET',
									label: 'RESET'
								},
								{
									value: '!DISABLE',
									label: 'DISABLE'
								},
								{
									value: '%23GETJPOS',
									label: 'GETJPOS'
								},
								{
									value: '%23GETLPOS',
									label: 'GETLPOS'
								},
								{
									value: '!REBOOT',
									label: 'REBOOT'
								},
								{
									value: '!STOP',
									label: 'STOP'
								}
							],
						}
					},
					mounted() {
						this.onload();
					},
					methods: {
						onload() {
							//this.getJointsLimit();
							this.getSysInfo();
						},
						getSysInfo() {
							axios.get('/getSysInfo').then(res => {
								console.log(res.data)
								this.ip = res.data.ip;
								this.ssid = res.data.ssid;
								this.wifiPassword = res.data.password;
								this.udpPort = res.data.udpPort;
								this.tcpPort = res.data.tcpPort;
								this.agentIp = res.data.rosAgentIp;
								this.agentPort = res.data.rosAgentPort;
								this.initWebsocket(this.ip)
							})
						},
						initWebsocket(ip) {
							this.websocket = new WebSocket('ws://' + ip + ':81');
							this.websocket.addEventListener('open', (event) => {
								console.log('WebSocket连接已建立');
								this.getJointsLimit();
								this.getPID();
							});
							this.websocket.addEventListener('message', (event) => {
										console.log('收到消息:', event.data);
									
										let data = event.data.split(' ')
										if (data.length < 1) return;
										if (data[0] === 'MoveJ'){
											if (this.cmdExecFlag === '执行') return;
											//if (this.cmdListRunMode === '2')
											  this.execRunList()
										}
										if (data[0] === 'pos') {
											console.log(data[1])
											this.J1Pos = parseFloat(data[1]);
											this.J2Pos = parseFloat(data[2]);
											this.J3Pos = parseFloat(data[3]);
											this.J4Pos = parseFloat(data[4]);
											this.J5Pos = parseFloat(data[5]);
											this.J6Pos = parseFloat(data[6]);
										}
										if (data[0] === 'posl') {
											console.log(data[1])
											this.X = parseFloat(data[1]);
											this.Y = parseFloat(data[2]);
											this.Z = parseFloat(data[3]);
											this.A = parseFloat(data[4]);
											this.B = parseFloat(data[5]);
											this.C = parseFloat(data[6]);
										}
										if (data[0] === 'JLA') {
											this.J1Min = parseFloat(data[1])
											this.J1Max = parseFloat(data[2])
											this.J2Min = parseFloat(data[3])
											this.J2Max = parseFloat(data[4])
											this.J3Min = parseFloat(data[5])
											this.J3Max = parseFloat(data[6])
											this.J2Pos = this.J2Min;
											this.J3Pos = this.J3Max;
										}
										if (data[0] === 'JLB') {
											this.J4Min = parseFloat(data[1])
											this.J4Max = parseFloat(data[2])
											this.J5Min = parseFloat(data[3])
											this.J5Max = parseFloat(data[4])
											this.J6Min = parseFloat(data[5])
											this.J6Max = parseFloat(data[6])
										}
										if (data[0] === 'KP'){
											this.motorPid[0].kp = parseFloat(data[1]);
											this.motorPid[1].kp = parseFloat(data[2]);
											this.motorPid[2].kp = parseFloat(data[3]);
											this.motorPid[3].kp = parseFloat(data[4]);
											this.motorPid[4].kp = parseFloat(data[5]);
											this.motorPid[5].kp = parseFloat(data[6]);
										}
										if (data[0] === 'KI'){
											this.motorPid[0].ki = parseFloat(data[1]);
											this.motorPid[1].ki = parseFloat(data[2]);
											this.motorPid[2].ki = parseFloat(data[3]);
											this.motorPid[3].ki = parseFloat(data[4]);
											this.motorPid[4].ki = parseFloat(data[5]);
											this.motorPid[5].ki = parseFloat(data[6]);
										}
										if (data[0] === 'KD'){
											this.motorPid[0].kd = parseFloat(data[1]);
											this.motorPid[1].kd = parseFloat(data[2]);
											this.motorPid[2].kd = parseFloat(data[3]);
											this.motorPid[3].kd = parseFloat(data[4]);
											this.motorPid[4].kd = parseFloat(data[5]);
											this.motorPid[5].kd = parseFloat(data[6]);
										}
										if (data[0] === 'HOME' || data[0] ==='RESET'){
											this.allowSendCmd = true;
											console.log('home ok')
										}
									})
								},
								getJointsLimit() {
									axios.get('/Dummy?cmd=%23GETJOINTSLIMITA').then(res => {
									})
									axios.get('/Dummy?cmd=%23GETJOINTSLIMITB').then(res => {
									})
								},
								onSetCmdMode(){
									let cmd = `/Dummy?cmd=%23CMDMODE ${this.cmdMode}`
									console.log(cmd)
									axios.get(cmd).then(res=>{
										console.log(res)
									})
								},
								onPosChange(value) {
									//if (!this.allowSendCmd) return;
									let url =
										`/Dummy?cmd=>${this.J1Pos},${this.J2Pos},${this.J3Pos},${this.J4Pos},${this.J5Pos},${this.J6Pos},${this.speed}`
										console.log(url)
									axios.get(url).then((res) => {
										console.log(res)
									}).catch((err) => {
										console.log(res)
									})
								},
								onInput(value) {
									this.$refs["j1"].setAttribute('rotation', {
										x: 0,
										y: this.J1Pos,
										z: 0
									})
									this.$refs["j2"].setAttribute('rotation', {
										x: 0,
										y: -90,
										z: -1 * this.J2Pos
									})
									this.$refs["j3"].setAttribute('rotation', {
										x: 180,
										y: 0,
										z: this.J3Pos - 90
									})
									this.$refs["j4"].setAttribute('rotation', {
										x: 0,
										y: 90,
										z: this.J4Pos
									})
									this.$refs["j5"].setAttribute('rotation', {
										x: 0,
										y: 90,
										z: this.J5Pos
									})
								},
								resetJoints() {
									this.J1Pos = 0;
									this.J2Pos = 0;
									this.J3Pos = 90;
									this.J4Pos = 0;
									this.J5Pos = 0;
									this.J6Pos = 0;
									this.onPosChange(0);
								},
								cmdRun() {
									console.log(this.ctrlCmd)
									this.allowSendCmd = false;
									axios.get('/Dummy?cmd=' + this.ctrlCmd).then((res) => {
										console.log(res)
									}).catch((err) => {
										console.log(res)
									})
								},
								clearCmdList(){
									this.dummyCmdList = [];
								},
								startRos(){
									axios.get(`/startRosNode?agentIp=${this.agentIp}&agentPort=${this.agentPort}`).then(res=>{
										console.log(res)
									})
								},
								sendCmd(cmd){
									return new Promise((resolve, reject) => {
										axios.get('/Dummy?cmd=' + cmd).then((res) => {
											resolve(res)
										})
									})
									
								},
								runCmdList(){
									if (this.dummyCmdList.length < 1) return;
									if (this.cmdExecFlag == '停止'){
										this.cmdExecFlag = '执行'
										this.cmdIndex = 0
										this.sendCmd('%23AUTO_SEND_POS false')
										return
									}
									this.execRunList()
									this.sendCmd('%23AUTO_SEND_POS true')
								},
								execRunList(){
									if (this.cmdIndex >= this.dummyCmdList.length) this.cmdIndex  = 0;
									let cmd = `/Dummy?cmd=${this.dummyCmdList[this.cmdIndex++].cmd}`
									axios.get(cmd).then(res=>{
										this.cmdExecFlag = '停止'
									})
								},
								addToCommandList() {
									let cmd =`>${this.J1Pos},${this.J2Pos},${this.J3Pos},${this.J4Pos},${this.J5Pos},${this.J6Pos},${this.speed}`
									this.dummyCmdList.push({
										executing: false,
										cmd: cmd
									})
								},
								animat() {
									console.log(this.$refs["j1"].setAttribute('rotation', '0 90 0'))
								},
								getPID() {
									this.sendCmd('%23GETKP')
									this.sendCmd('%23GETKI')
									this.sendCmd('%23GETKD')
								},
								resetWifi(){
									axios.get(`/setWifi?ssid=${this.ssid}&pass=${this.wifiPassword}`).then(res=>{
										console.log(res)
									})
								},
								onSetMotorPID(node) {
									let cmdKp = `%23SET_DCE_KP ${node} ${this.motorPid[node -1].kp}`
									let cmdKi = `%23SET_DCE_KI ${node} ${this.motorPid[node -1].ki}`
									let cmdKd = `%23SET_DCE_KD ${node} ${this.motorPid[node -1].kd}`
									this.sendCmd(cmdKp).then(res=>{
										this.sendCmd(cmdKi).then(resi=>{
											this.sendCmd(cmdKd)
										})
									})
								},
								refreshJointsL(){
									this.sendCmd('%23GETLPOS')
								},
								resetJointsL(){
									this.X = 0;
									this.Y = 0;
									this.Z = 0;
									this.A = 0;
									this.B = 0;
									this.C = 0;
									this.sendXYZ()
									console.log('resetJoinsL')
								},
								sendXYZ(){
									let cmdMoveL = `@${this.X},${this.Y},${this.Z},${this.A},${this.B},${this.C},${this.speed}`
									this.sendCmd(cmdMoveL)
								},
								addXYZToCommandList(){
									let cmd= `@${this.X},${this.Y},${this.Z},${this.A},${this.B},${this.C},${this.speed}`;
									this.dummyCmdList.push({
										executing: false,
										cmd: cmd
									})
								}
						}
					})
	</script>
</html>
)==";

// used for $upload.htm
static const char notFoundContent[] PROGMEM = R"==(
<html>
<head>
  <title>Resource not found</title>
</head>
<body>
  <p>The resource was not found.</p>
  <p><a href="/">Start again</a></p>
</body>
</html>
)==";
