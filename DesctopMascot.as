/**
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
package jp.live2d.sample 
{
	import flash.display.NativeWindow;
	import flash.display.Stage;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.InteractiveObject;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.display.Stage3D;
	import flash.display.DisplayObjectContainer;
	import flash.display.DisplayObject;
	import flash.system.Capabilities;
	import flash.display3D.Context3D;
	import flash.display3D.Context3DRenderMode;
	import flash.events.ContextMenuEvent;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.text.AntiAliasType;
	import flash.text.TextFieldAutoSize;
	import flash.text.GridFitType;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.ui.ContextMenu;
	import flash.ui.ContextMenuItem;
	import flash.utils.ByteArray;
	import flash.utils.Timer;
	import jp.live2d.Live2D;
	import jp.live2d.Live2DModelAs3;
	import jp.live2d.motion.Live2DMotion;
	import jp.live2d.motion.MotionQueueManager;
	import jp.live2d.util.UtSystem;
	[SWF(width="512", height="512", backgroundColor="0xFFFFFF", frameRate="30")]
	public class DesctopMascot extends Sprite
	{
		
		public var context3D:Context3D;
		
		public var live2DModel:Live2DModelAs3;
		public var motionMgr:MotionQueueManager;
		public var motions:Vector.<Live2DMotion>;
		public var offscreen:Bitmap;
		
		[Embed(source = '../../../../assets/haru/haru.moc', mimeType = 'application/octet-stream')] private var ModelData:Class;
		[Embed(source = '../../../../assets/haru/haru.1024/texture_00.png')] private var Texture_00:Class;
		[Embed(source = '../../../../assets/haru/haru.1024/texture_01.png')] private var Texture_01:Class;
		[Embed(source = '../../../../assets/haru/haru.1024/texture_02.png')] private var Texture_02:Class;
		
		public var textures:Array = [Texture_00,Texture_01,Texture_02];
		
		
		[Embed(source = '../../../../assets/wanko/motions/idle_01.mtn', mimeType = 'application/octet-stream')] private var Motion_00:Class;
		[Embed(source = '../../../../assets/wanko/motions/idle_02.mtn', mimeType = 'application/octet-stream')] private var Motion_01:Class;
		[Embed(source = '../../../../assets/wanko/motions/idle_03.mtn', mimeType = 'application/octet-stream')] private var Motion_02:Class;
		[Embed(source = '../../../../assets/wanko/motions/idle_04.mtn', mimeType = 'application/octet-stream')] private var Motion_03:Class;
		
		
		public var motionClass:Array = [Motion_00, Motion_01, Motion_02, Motion_03];
		
		public function DesctopMascot():void
		{
			Live2D.init();
			var stage3D:Stage3D = stage.stage3Ds[0];
			
			
			// 3Dレイヤーの準備
			stage3D.addEventListener(Event.CONTEXT3D_CREATE, onCreateStage3D, false, 1);
			stage3D.requestContext3D(Context3DRenderMode.AUTO);
			
		}
		
		
		/*
		 * 3Dレイヤーの準備完了時
		 * @param	event
		 */
		public function onCreateStage3D(e:Event):void
		{
			var stage3D:Stage3D = (e.target as Stage3D);
			context3D = stage3D.context3D;
			context3D.configureBackBuffer(stage.stageWidth, stage.stageHeight, 0, false);
			context3D.setRenderToBackBuffer();
			
			live2DModel = Live2DModelAs3.loadModel( ByteArray(new ModelData) );
			
			// グラフィックコンテキストをモデルに設定
			live2DModel.setGraphicsContext(context3D);
			
			// テクスチャの設定
			for (var i:int = 0; i < textures.length; i++) 
			{
				var tex:Bitmap = new textures[i];
				live2DModel.setTexture(i, tex.bitmapData );				
			}
			
			// 「アプリ起動時に作成されるウィンドウ」を取得
			var window:NativeWindow = stage.nativeWindow;
			
			//ディスプレイの解像度を取得
			var screenW:int = Capabilities.screenResolutionX
			var screenH:int = Capabilities.screenResolutionY
			
			// ウィンドウが保有するステージオブジェクトを取得
			var stage_obj:Stage = window.stage;

			// 位置を変更する
			window.x = screenW - stage_obj.stageWidth + 150;
			window.y = screenH - stage_obj.stageHeight; 
			
		/*	// 位置を変更する
			window.x = screenW - window.width;
			window.y = screenH - window.height; */
			
			// 「任意の表示オブジェクト」を表示リストから除外する関数
			function DisplayObjectRemove(display_obj:DisplayObject) :Boolean{
			var parent:DisplayObjectContainer = display_obj.parent;
			if(!parent)	return false;
			var obj:DisplayObject =  parent.removeChild(display_obj);
			return (obj == display_obj);
			}
			
			// 表示位置の設定
			var scale:Number = 2 / live2DModel.getCanvasWidth(); // 横幅で合わせる
			live2DModel.scaleX = scale;
			live2DModel.scaleY = -scale;
			live2DModel.x = -1; // スクリーン上の左上とモデルの左上を合わせる
			live2DModel.y = 1;
			
			 /*// ボタン用(textのはラベル) 
			var button1:SimpleButton;
			var buttonText1:TextField; */
			
			motionMgr = new MotionQueueManager();
			motions = new Vector.<Live2DMotion>();
			
			for (i = 0; i < motionClass.length; i++) 
			{
				var motion:Live2DMotion = Live2DMotion.loadMotion(String(new motionClass[i]));
				motions.push(motion);
			}
			
			var dragSprite:Sprite = new Sprite;
			
			offscreen = new Bitmap(new BitmapData(stage.stageWidth, stage.stageHeight, true, 0x00FFFFFF));
			dragSprite.addEventListener(MouseEvent.MOUSE_DOWN,function ():void 
			{
				stage.nativeWindow.startMove();
			});
			addChild(dragSprite);
			dragSprite.addChild(offscreen);
			stage3D.visible = false;
			
			stage.nativeWindow.alwaysInFront = true;
			
			addMenu(this,"簡易ガチャシミュレーター",function ():void 
			{ 
				
				var gacha:TextField = new TextField();
				
				gacha.x = 10;
				gacha.y = 10;
				gacha.width  = stage.stageWidth  - 330;
				gacha.height = stage.stageHeight - 350;
				// ステージの表示リストに登録する
				stage.addChild(gacha);
				
				var num:int = 0;
				var countup:int = 0;
				while(true){
				num = Math.floor( Math.random() * 100 );
				countup++;
					if (num == 1 || num == 2 || num == 3){
					break;
					}
				}
				//回数を文字列に
				var countstr:String = String(countup);
				
				//かかったお金
				var money:int = (300 * countup);
				
				gacha.text = (countstr);
				gacha.appendText('回ガチャを回したらSSRが出たよ。\n');
				gacha.text += (money);
				gacha.appendText('円かかったよ。');
				
				gacha.background = true;
				gacha.backgroundColor = 0xFF8C00;
				gacha.border = false;
				
				//4秒後にtextfieldを消す
				var timer : Timer = new Timer(4000, 1);
				timer.addEventListener(TimerEvent.TIMER_COMPLETE,function (e:TimerEvent):void{
				DisplayObjectRemove(gacha);
				});
				timer.start();

				
			});
			
			addMenu(this,"グラブルwiki",function ():void 
			{ 
				　var url:URLRequest = new URLRequest( "http://gbf-wiki.com/" );
				　navigateToURL( url );
			});
			addMenu(this,"グラブル計算機",function ():void 
			{ 
				　var url:URLRequest = new URLRequest( "http://hibin0.web.fc2.com/grbr_atk_calc/atk_calc.html" );
				　navigateToURL( url );
			});
			/*addMenu(this,"デバフタイマー",function ():void 
			{ 
			}); */
			
			addMenu(this,"テキストテスト",function ():void 
			{ 
				// textfieldをインスタンス化
				var text_field:TextField = new TextField();
			
				// サイズとか
				text_field.x = 10;
				text_field.y = 10;
				text_field.width  = stage.stageWidth  - 330;
				text_field.height = stage.stageHeight - 350;
				// ステージの表示リストに登録する
				stage.addChild(text_field);
				
				//************************************************************
				
				/*time--;
				if(time <=0 ) { */
                
				//0から5の範囲でランダムな値が生成
				var num:int = Math.floor( Math.random() * 4 );
            	
				if (num == 0)
				{
					text_field.text = "こんにちは！";
				}
				
				if (num == 1)
				{
					text_field.text = "おはよう！";
				}
				
				if (num == 2)
				{
					text_field.text = "よっしゃ！";
				}
				
				if (num == 3)
				{
					text_field.text = "おつかれ！";
				}
                //time = 30;
								
				
				//textfieldのプロパティ
				text_field.background = true;
				text_field.backgroundColor = 0xFF8C00;
				text_field.border = false;
				
				//5秒後にtextfieldを消す
				var timer : Timer = new Timer(4000, 1);
				timer.addEventListener(TimerEvent.TIMER_COMPLETE,function (e:TimerEvent):void{
				DisplayObjectRemove(text_field);
				});
				timer.start();
				
			});
			
			// 吹き出し用のムービークリップとマスクを用意しないとダメ
			/*addMenu(this,"吹き出しテスト",function ():void 
			{ 
			//ふきだし生成
			function makeFrm(num) {
			　　//ふきだしおよびテキストフォーマット＆フィールドを作る
			　　var frm = new frame();
			　　var frt = new TextFormat();
			　　var txt = new TextField();
			　　var wid = frm.width / 2;
			　　//ふきだしの初期値
			　　frm.alpha = 0.5;
			　　frm.y = hit[num].y - (hit[num].height / 2);
			　　//テキストフォーマット初期値
			　　frt.size  = 13;
			　　frt.align = TextFormatAlign.CENTER;
			　　//テキストフィールド初期値
			　　txt.defaultTextFormat = frt;
			　　txt.width = 170;
			　　txt.height = 20;
			　　txt.x -= 85;
			　　txt.y -= 35;
			　　//表示リストに登録
　　			addChild(frm);
			　　frm.addChild(txt);
　			　//リムーブ用変数に代入
　			　//mc = frm; 
				}
			});
			*/
			
			
			
			addMenu(this,"最小化",function ():void 
			{ 
				window.minimize();
			});
			
			addMenu(this,"アプリケーションの終了",function ():void 
			{ 
				stage.nativeWindow.close();
			});
			
			stage.addEventListener(Event.ENTER_FRAME, update);
		}
		
		
		public function update(e:Event):void
		{
			if (motionMgr.isFinished()) 
			{
				var idle:Live2DMotion = motions[int(Math.random()*motions.length)];
				motionMgr.startMotion(idle);
			}
			else 
			{
				motionMgr.updateParam(live2DModel);
			}
			
			
			live2DModel.update(); // モデルパラメータの更新
			
			context3D.clear(0, 0, 0, 0); // 画面のクリア
			live2DModel.draw(); // モデル描画
			
			context3D.drawToBitmapData(offscreen.bitmapData);
			
			context3D.present(); // 描画を適用
		}
		
		/*
		 * コンテストメニューに項目と実行関数を登録する
		 */
		static public function addMenu(target:InteractiveObject,caption:String, func:Function=null,checked:Boolean=false):void {
			var menu_item:ContextMenuItem = new ContextMenuItem("");	// メニューアイテムを作成

			menu_item.caption = caption;		// キャプション名
			if(func!=null){
				menu_item.enabled = true;			// 有効か
				menu_item.addEventListener(ContextMenuEvent.MENU_ITEM_SELECT, func);
			}else {
				menu_item.enabled = false;			
			}
			menu_item.separatorBefore = false;		// １つ上にセパレータを付けるか
			menu_item.visible = true;			// 可視表示するか

			//ブラウザ版では使えない
			//menu_item.checked = checked;
			if (menu_item.hasOwnProperty("checked")) 
			{
				menu_item["checked"] = checked;
			}
			
			if(target.contextMenu==null){
				var menu_cm:ContextMenu = new ContextMenu ();
				menu_cm.customItems = [menu_item];		// カスタムメニューに登録
				
				// スプライトにコンテキストメニューを登録
				target.contextMenu = menu_cm;
				
			}else {
				// Flashではtarget.contextMenuはContextMenu
				// Airではtarget.contextMenuはNativeMenu（ContextMenuのスーパークラス）
				
				//target.contextMenu.hideBuiltInItems();
				if (target.contextMenu is ContextMenu) {
					if (ContextMenu(target.contextMenu).customItems==null) 
					{
						ContextMenu(target.contextMenu).customItems = new Array();
					}
					ContextMenu(target.contextMenu).customItems.push(menu_item);
				}
			}
			//target.contextMenu.hideBuiltInItems();
		}
	}

}