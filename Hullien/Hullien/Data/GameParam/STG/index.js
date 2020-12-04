var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

class CBullet
{
	constructor( x, y )
	{
		this.x			= x;
		this.y			= y;
		this.f_w		= canvas.width;
		this.f_h		= canvas.height;
		this.isShot		= false;
		this.moveSpeed	= 0.0;
		this.vec_x		= 0.0;
		this.vec_y		= 0.0;
	}
	
	shoot( rot, s ){
		if( this.isShot == true ) return;
		this.isShot = true;
		this.moveSpeed = s;
		this.vec_x = Math.sin(rot);
		this.vec_y = Math.cos(rot);
	}
	
	update( ctx )
	{
//		if( this.isShot == false ) return;
		this.x += this.vec_x * this.moveSpeed;
		this.y += this.vec_y * this.moveSpeed;
		
		ctx.beginPath();
		ctx.arc( this.x, this.y, 3, 360*Math.PI/180.0, false );
		ctx.fillStyle = "red";
		ctx.fill();
		ctx.closePath();
		
		return;
	}
}

class CData
{
	constructor(
		index,
		bulletSpeed,
		bulletNum,
		nWayNum,
		angle,
		nWayAngle,
		bulletAngleAddAccValue,
		intrval,
		dips )
	{
		this.index			= index;	
		this.bulletSpeed	= bulletSpeed;
		this.bulletNum		= bulletNum;
		this.nWayNum		= nWayNum;
		this.angle			= angle;
		this.nWayAngle		= nWayAngle;
		this.bulletAngleAddAccValue = bulletAngleAddAccValue;
		this.intrval		= intrval;
		this.dips			= dips;
	}
}


var isPlay = false;	// 再生しているかどうか.
var b = [];
var shotFrame = 0;
var shotCount = 0;
var shotAngle = 0;
var bulletAddAngle = 0;
var bulletAngleAccValue = 0;
var bulletAngleAddAccValue = 0;

//-------------------------------------------------.
// ボタンを押したときのコールバックを設定.
//-------------------------------------------------.

// 再生.
document.getElementById("play").onclick = function () {
	//------------------------------------------.
	// パラメータの取得.
	data.index					= Number(document.getElementById("index").value);
	data.bulletSpeed			= Number(document.getElementById("bulletSpeed").value)*20;
	data.bulletNum				= Number(document.getElementById("bulletNum").value);
	data.nWayNum				= Number(document.getElementById("nWayNum").value);
	data.angle					= Number(document.getElementById("angle").value) * (Math.PI / 180.0);
	data.nWayAngle				= Number(document.getElementById("nWayAngle").value) * (Math.PI / 180.0);
	data.bulletAngleAddAccValue	= Number(document.getElementById("angleAddAccValue").value) * (Math.PI / 180.0);
	data.intrval				= Number(document.getElementById("intrval").value);
	data.dips					= Number(document.getElementById("dips").value);
	
	// 弾の初期化.
	for (var i = 0; i < data.bulletNum; i++) {
		b[i] = new CBullet(240, 100);
	}
	isPlay = true;
	shotFrame = 0;
	shotCount = 0;
	shotAngle = 0;
	bulletAddAngle = data.angle;
	bulletAngleAccValue = 0;
	bulletAngleAddAccValue = data.bulletAngleAddAccValue;
}

// 停止.
document.getElementById("stop").onclick = function () {
	isPlay = false;
}

// データのコピー.
document.getElementById("copy").onclick = function () {
	// データの取得.
	data.index					= Number(document.getElementById("index").value);
	data.bulletSpeed			= Number(document.getElementById("bulletSpeed").value);
	data.bulletNum				= Number(document.getElementById("bulletNum").value);
	data.nWayNum				= Number(document.getElementById("nWayNum").value);
	data.angle					= Number(document.getElementById("angle").value);
	data.nWayAngle				= Number(document.getElementById("nWayAngle").value);
	data.bulletAngleAddAccValue	= Number(document.getElementById("angleAddAccValue").value);
	data.intrval				= Number(document.getElementById("intrval").value);
	data.dips					= Number(document.getElementById("dips").value);
	var target =
		String(data.index)					+ "\n" +
		String(data.bulletSpeed)			+ "\n" +
		String(data.bulletNum)				+ "\n" +
		String(data.nWayNum)				+ "\n" +
		String(data.angle)					+ "\n" +
		String(data.nWayAngle)				+ "\n" +
		String(data.bulletAngleAddAccValue)	+ "\n" +
		String(data.intrval)				+ "\n" +
		String(data.dips);

	// textareaを作成.
	var area = document.createElement("textarea");	
	area.textContent = target;
	document.body.appendChild(area);

	// 選択/コピーする.
	area.select();
	document.execCommand("Copy");

	// 生成したtextareaを削除.
	document.body.removeChild(area);
}

var data = new CData(1, 0.1, 120, 5, 36, 12, 1, 10, 1);

document.getElementById("index").value				= data.index;
document.getElementById("bulletSpeed").value		= data.bulletSpeed;
document.getElementById("bulletNum").value			= data.bulletNum;
document.getElementById("nWayNum").value			= data.nWayNum;
document.getElementById("angle").value				= data.angle;
document.getElementById("nWayAngle").value			= data.nWayAngle;
document.getElementById("angleAddAccValue").value	= data.bulletAngleAddAccValue;
document.getElementById("intrval").value			= data.intrval;
document.getElementById("dips").value				= data.dips;

function main()
{
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	
	// 再生していた場合弾などの更新をする.
	if (isPlay == true) {
		//------------------------------------------.
		if (shotFrame % data.intrval == 0) {
			var n;
			var a = shotAngle;
			for (n = 0; n < data.nWayNum; n++) {
				if (shotCount < data.bulletNum) {
					b[shotCount].shoot(a, data.bulletSpeed);
					a += data.nWayAngle;
				} else {
					if (shotCount != data.bulletNum) {
						isStart = false;
						alert("弾数とnWay数が合いません");
					}
					break;
				}
				shotCount++;
			}
			shotAngle += bulletAddAngle;	// 角度の加算.
			bulletAddAngle += bulletAngleAccValue;
			bulletAngleAccValue += bulletAngleAddAccValue;
			
			if (Math.abs(bulletAddAngle) > Math.abs(data.angle)) {
				if (bulletAngleAccValue > 0.0) {
					bulletAddAngle = Math.abs(data.angle);
					bulletAngleAddAccValue = -bulletAngleAddAccValue;
					bulletAngleAccValue = 0.0;
				} else {
					bulletAddAngle = -Math.abs(data.angle);
					bulletAngleAddAccValue = -bulletAngleAddAccValue;
					bulletAngleAccValue = 0.0;
				}
			}

		}
		//------------------------------------------.
		
		// 弾の更新.
		for( var i = 0; i < shotCount; i++ ){
			b[i].update( ctx );
		}
		// フレームが一定値に達したら終了.
		if (shotFrame == (data.intrval * data.bulletNum) + 100) {
			isStart = false;
		}
		shotFrame++;
	}
}

var interval = setInterval( main, 10 );