var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

if (window.File) {
	// File APIに関する処理を記述
	window.alert("File APIが実装されてます。");
} else {
	window.alert("本ブラウザではFile APIが使えません");
}

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
		no,			text,
		textSize,	hp,
		moveSpeed,	bulletSpeed,
		bulletNum,	nWayNum,
		angle,		nWayAngle,
		intrval,	dips )
	{
		this.no				= no;
		this.text			= text;
		this.textSize		= textSize;
		this.hp				= hp;
		this.moveSpeed		= moveSpeed;
		this.bulletSpeed	= bulletSpeed;
		this.bulletNum		= bulletNum;
		this.nWayNum		= nWayNum;
		this.angle			= angle;
		this.nWayAngle		= nWayAngle;
		this.intrval		= intrval;
		this.dips			= dips;
	}
}


var isPlay = false;	// 再生しているかどうか.
var b = [];
var shotFrame = 0;
var shotCount = 0;
var shotAngle = 0;

//-------------------------------------------------.
// ボタンを押したときのコールバックを設定.
//-------------------------------------------------.

// 再生.
document.getElementById("play").onclick = function () {
	//------------------------------------------.
	// パラメータの取得.
	data.no = document.getElementById("no").value;
	data.text = document.getElementById("text").value;
	data.textSize = document.getElementById("textsize").value;
	data.hp = Number(document.getElementById("hp").value);
	data.moveSpeed = Number(document.getElementById("moveSpeed").value);
	data.bulletSpeed = Number(document.getElementById("bulletSpeed").value);
	data.bulletNum = Number(document.getElementById("bulletNum").value);
	data.nWayNum = Number(document.getElementById("nWayNum").value);
	data.angle = Number(document.getElementById("angle").value) * (Math.PI / 180.0);
	data.nWayAngle = Number(document.getElementById("nWayAngle").value) * (Math.PI / 180.0);
	data.intrval = Number(document.getElementById("intrval").value);
	data.dips = Number(document.getElementById("dips").value);

	// 弾の初期化.
	for (var i = 0; i < data.bulletNum; i++) {
		b[i] = new CBullet(240, 100);
	}
	isPlay = true;
	shotFrame = 0;
	shotCount = 0;
	shotAngle = 0;
}

// 停止.
document.getElementById("stop").onclick = function () {
	isPlay = false;
}

// 保存.
document.getElementById("save").onclick = function () {

}

var data = new CData(
	1, "Test",
	1, 5,
	1, 1,
	30, 3,
	0, 15,
	10, 1);

document.getElementById("no").value = data.no;
document.getElementById("text").value = data.text;
document.getElementById("textsize").value = data.textSize;
document.getElementById("hp").value = data.hp;
document.getElementById("moveSpeed").value = data.moveSpeed;
document.getElementById("bulletSpeed").value = data.bulletSpeed;
document.getElementById("bulletNum").value = data.bulletNum;
document.getElementById("nWayNum").value = data.nWayNum;
document.getElementById("angle").value = data.angle;
document.getElementById("nWayAngle").value = data.nWayAngle;
document.getElementById("intrval").value = data.intrval;
document.getElementById("dips").value = data.dips;

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
			shotAngle += data.angle;	// 角度の加算.
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
	
	//----------------------------------------.
	// デバッグテキスト.
	ctx.font = "16px Arial";
	ctx.fillStyele = "#0095DD";
	ctx.fillText(data.no,			8, 20 );
	ctx.fillText(data.text,			8, 40 );
	ctx.fillText(data.textsize,		8, 60 );
	ctx.fillText(data.hp,			8, 80 );
	ctx.fillText(data.moveSpeed,	8, 100 );
	ctx.fillText(data.bulletSpeed,	8, 120 );
	ctx.fillText(data.bulletNum,	8, 140 );
	ctx.fillText(data.nWayNum,		8, 160 );
	ctx.fillText(data.angle,		8, 180 );
	ctx.fillText(data.nWayAngle,	8, 200 );
	ctx.fillText(data.intrval,		8, 220 );
	ctx.fillText(data.dips,			8, 240 );
}

var interval = setInterval( main, 10 );