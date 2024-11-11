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
		<title>Dummy Web控制台 V0.1 pre-alpha</title>
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

				.clearfix:before,
				.clearfix:after {
					display: table;
					content: "";
				}

				.clearfix:after {
					clear: both
				}

				.box-card {
					width: 100%;
					height: 150px;
				}

				.pidValue {
					max-width: 5.125rem;
				}
		</style>
	</head>
	<body style="background-color: #E9EEF3;">

		<div id="app">
			<el-container>
				<el-header>Dummy Web 控制台 V0.1 Pre-Alpha</el-header>
				<el-main>
					<el-row :gutter="6">
						<el-row :gutter="6">
							<el-col :span="12">
								<el-card class="box-card">
									<div slot="header" class="clearfix">
										<span>命令</span>
									</div>
									<el-row style="margin-top: 10px;">
										<el-col :span="12">
											<el-select v-model="ctrlCmd" placeholder="请选择命令">
												<el-option v-for="item in ctrlCmdOptions" :key="item.value"
													:label="item.label" :value="item.value">
												</el-option>
											</el-select>
											<el-button @click="cmdRun" type="primary">发送</el-button>

										</el-col>
										<el-col :span="12">
											<span>命令模式</span>
											<el-input-number v-model="J6Pos" :controls="false" :min="-360"
												:max="360"></el-input-number>
											<el-button @click="visible = true" type="primary">设置</el-button>
										</el-col>
									</el-row>
								</el-card>
								<el-card style="height: 180px;margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>运行</span>
									</div>
									<el-row :gutter="10">
										<el-col :span="4">
											<span>J1</span>
											<el-input-number v-model="J1Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
										<el-col :span="4">
											<span>J2</span>
											<el-input-number v-model="J2Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
										<el-col :span="4">
											<span>J3</span>
											<el-input-number v-model="J3Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
										<el-col :span="4">
											<span>J4</span>
											<el-input-number v-model="J4Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
										<el-col :span="4">
											<span>J5</span>
											<el-input-number v-model="J5Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
										<el-col :span="4">
											<span>J6</span>
											<el-input-number v-model="J6Pos" :min="-360" :max="360" size="mini"
												:controls="false"></el-input-number>
										</el-col>
									</el-row>
									<el-row style="margin-top: 15px;">
										<el-button @click="visible = true" type="primary">发送到Dummy</el-button>
										<el-button @click="visible = true" type="primary">添加到命令列表</el-button>
									</el-row>
								</el-card>
								<el-card style="height: 465px; margin-top: 6px;">
									<div slot="header" class="clearfix">
										<span>命令列表</span>
										<div style="float: right;">
											<el-radio v-model="cmdListRunMode" label="0">单步运行</el-radio>
											<el-radio v-model="cmdListRunMode" label="1">单次运行</el-radio>
											<el-radio v-model="cmdListRunMode" label="2">循环运行</el-radio>
											<el-button @click="cmdRun" type="primary">执行</el-button>
											<el-button @click="animat" type="primary">导入命令</el-button>
										</div>
									</div>
									<ol>
										<li v-for="i in dummyCmdList" class="infinite-list-item">{{ i }}</li>
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
														type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J2</span>
													<el-button style="float: right; padding: 3px 0"
														type="text">设置</el-button>
												</div>

												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J3</span>
													<el-button style="float: right; padding: 3px 0"
														type="text">设置</el-button>
												</div>

												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
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
														type="text">设置</el-button>
												</div>

												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J5</span>
													<el-button style="float: right; padding: 3px 0"
														type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
												</el-row>
											</el-card>
										</el-col>
										<el-col :span="8">
											<el-card>
												<div slot="header" class="clearfix">
													<span>J6</span>
													<el-button style="float: right; padding: 3px 0"
														type="text">设置</el-button>
												</div>
												<el-row>
													<el-col :span="8">
														<span>P</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>I</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
													</el-col>
													<el-col :span="8">
														<span>D</span>
														<el-input-number class="pidValue" v-model="J6Pos"
															:controls="false" :min="-360" :max="360"></el-input-number>
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
										<el-checkbox v-model="checked"
											style="float: right; padding: 3px 0">录制</el-checkbox>
									</div>
									<div id="virtualSceneContainer" style="width: 100%;height: 820px;">
										<a-scene style="width: 100%;height: 100%;" embedded background="color: #212"
											environment>
											<a-assets>
												<a-asset-item id="base"  src="http://qiniu.wansuiwht.top/Model/base.glb"></a-asset-item>
												<a-asset-item id="j1" src="http://qiniu.wansuiwht.top/Model/j1.glb"></a-asset-item>
												<a-asset-item id="j2" src="http://qiniu.wansuiwht.top/Model/j2j3.glb"></a-asset-item>
												<a-asset-item id="j4" src="http://qiniu.wansuiwht.top/Model/j4.glb"></a-asset-item>
												<a-asset-item id="j5" src="http://qiniu.wansuiwht.top/Model/j5.glb"></a-asset-item>
												<a-asset-item id="j6" src="http://qiniu.wansuiwht.top/Model/j6.glb"></a-asset-item>
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
														<a-entity ref="j4" gltf-model="#j4" modify-materials
															position="0 0.146 -0.033" rotation="180 0 0">
															<a-entity ref="j5" gltf-model="#j5" modify-materials
																position="0.015 -0.05 -0.02" rotation="0 90 0">
																<a-entity ref="j6" gltf-model="#j6" modify-materials
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

	<!-- import Vue before Element -->
	<script src="https://unpkg.com/vue@2/dist/vue.js"></script>
	<script src="https://unpkg.com/axios/dist/axios.min.js"></script>

	<!-- import JavaScript -->
	<script src="https://unpkg.com/element-ui@2.15.14/lib/index.js"></script>
	<script setup type="module">
		new Vue({
			el: '#app',
			data: function() {
				return {
					visible: false,
					dummyLog: "started ",
					dummyCmdList: ['&0,0,90,0,0,0', '&0,0,90,50,0,0'],
					cmdListRunMode: '0',
					virtualSceneContainer: null,
					J1Pos: 0,
					J2Pos: 0,
					J3Pos: 90,
					J4Pos: 0,
					J5Pos: 0,
					J6Pos: 0,
					ctrlCmd: '',
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
							value: '!GETJPOS',
							label: 'GETJPOS'
						},
						{
							value: '!GETLPOS',
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

				},
				cmdRun() {
					console.log(this.ctrlCmd)
					axios.get('/Dummy?cmd='+this.ctrlCmd).then((res)=>{
						console.log(res)
					}).catch((err)=>{
						console.log(res)
					})
				},
				animat() {
					console.log(this.$refs["j1"].setAttribute('rotation', '0 90 0'))
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
)==";
