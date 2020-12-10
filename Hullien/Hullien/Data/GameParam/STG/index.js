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

class CWepon
{
    constructor( data )
    {
        this.data = data;
        this.b = [];
        this.shotFrame = 0;
        this.shotCount = 0;
        this.shotAngle = 0;
        this.bulletAddAngle = 0;
        this.bulletAngleAccValue = 0;
        this.bulletAngleAddAccValue = 0;
    }

    init()
    {
        // 弾の初期化.
        for (var i = 0; i < this.data.bulletNum; i++) {
            this.b[i] = new CBullet(240, 100);
        }
        this.isPlay = true;
        this.shotFrame = 0;
        this.shotCount = 0;
        this.shotAngle = 0;
        this.bulletAddAngle = this.data.angle;
        this.bulletAngleAccValue = 0;
        this.bulletAngleAddAccValue = this.data.bulletAngleAddAccValue;

        return this.data.bulletNum * this.data.intrval + 10;
    }

    update( ctx )
    {
        //------------------------------------------.
        if (this.shotFrame % this.data.intrval == 0) {
			var n;
			var a = this.shotAngle;
			for (n = 0; n < this.data.nWayNum; n++) {
			    if (this.shotCount < this.data.bulletNum) {
			        this.b[this.shotCount].shoot(a, this.data.bulletSpeed);
			        a += this.data.nWayAngle;
				} else {
			        if (this.shotCount != this.data.bulletNum) {
						isStart = false;
						alert("弾数とnWay数が合いません");
					}
					break;
				}
			    this.shotCount++;
			}
			this.shotAngle += this.bulletAddAngle;	// 角度の加算.
			this.bulletAddAngle += this.bulletAngleAccValue;
			this.bulletAngleAccValue += this.bulletAngleAddAccValue;
			
			if (Math.abs(this.bulletAddAngle) > Math.abs(this.data.angle)) {
			    if (this.bulletAngleAccValue > 0.0) {
			        this.bulletAddAngle = Math.abs(this.data.angle);
			        this.bulletAngleAddAccValue = -this.bulletAngleAddAccValue;
			        this.bulletAngleAccValue = 0.0;
				} else {
			        this.bulletAddAngle = -Math.abs(this.data.angle);
			        this.bulletAngleAddAccValue = -this.bulletAngleAddAccValue;
			        this.bulletAngleAccValue = 0.0;
				}
			}

		}
		//------------------------------------------.
		
		// 弾の更新.
        for (var i = 0; i < this.shotCount; i++) {
            this.b[i].update(ctx);
		}
        this.shotFrame++;
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
var endCount = 0;   // 終了カウント.
var endFrame = 0;   // 終了フレーム.
var wepon = [];
var weponNum = 0;   

//-------------------------------------------------.
// ボタンを押したときのコールバックを設定.
//-------------------------------------------------.

// 再生.
document.getElementById("play").onclick = function () {
    wepon = [];
    //------------------------------------------.
    // パラメータの取得.
    data.index                  = Number(document.getElementById("index").value);
    data.bulletSpeed            = Number(document.getElementById("bulletSpeed").value) * 20;
    data.bulletNum              = Number(document.getElementById("bulletNum").value);
    data.nWayNum                = Number(document.getElementById("nWayNum").value);
    data.angle                  = Number(document.getElementById("angle").value) * (Math.PI / 180.0);
    data.nWayAngle              = Number(document.getElementById("nWayAngle").value) * (Math.PI / 180.0);
    data.bulletAngleAddAccValue = Number(document.getElementById("angleAddAccValue").value) * (Math.PI / 180.0);
    data.intrval                = Number(document.getElementById("intrval").value);
    data.dips                   = Number(document.getElementById("dips").value);

    // 弾の初期化.
    for (var i = 0; i < 1; i++) {
        wepon[i] = new CWepon(data);
        endFrame = wepon[i].init();
    }
    isPlay = true;
    endCount = 0;
    weponNum = wepon.length;
}

// 停止.
document.getElementById("stop").onclick = function () {
	isPlay = false;
}

// 再生.
document.getElementById("anyPlay").onclick = function () {
    wepon = [];
    //------------------------------------------.
    // パラメータの取得.
    var table = document.getElementById("addTable");

    var frame = 0;
    var i = 0;
    var onceSkip = false;
    for(var row of table.rows) {
        if (onceSkip == false)
        {
            onceSkip = true;
            continue;
        }
        var tmpDate = new CData(1, 0.1, 120, 5, 36, 12, 1, 10, 1);
        tmpDate.index       = Number(row.cells[0].innerHTML);
        tmpDate.bulletSpeed = Number(row.cells[1].innerHTML) * 20;
        tmpDate.bulletNum   = Number(row.cells[2].innerHTML);
        tmpDate.nWayNum     = Number(row.cells[3].innerHTML);
        tmpDate.angle       = Number(row.cells[4].innerHTML) * (Math.PI / 180.0);
        tmpDate.nWayAngle   = Number(row.cells[5].innerHTML) * (Math.PI / 180.0);
        tmpDate.bulletAngleAddAccValue  = Number(row.cells[6].innerHTML) * (Math.PI / 180.0);
        tmpDate.intrval                 = Number(row.cells[7].innerHTML);
        tmpDate.dips                    = Number(row.cells[8].innerHTML);

        wepon[i] = new CWepon(tmpDate);
        frame = wepon[i].init();
        if (frame > endFrame) endFrame = frame;
        i++;
    }

    isPlay = true;
    weponNum = wepon.length;
    endCount = 0;
}

// 停止.
document.getElementById("anyStop").onclick = function () {
    isPlay = false;
}

// データのコピー.
document.getElementById("copy").onclick = function () {
    // データの取得.
    var tmpDate = new CData(1, 0.1, 120, 5, 36, 12, 1, 10, 1);
    tmpDate.index           = Number(document.getElementById("index").value);
    tmpDate.bulletSpeed     = Number(document.getElementById("bulletSpeed").value);
    tmpDate.bulletNum       = Number(document.getElementById("bulletNum").value);
    tmpDate.nWayNum         = Number(document.getElementById("nWayNum").value);
    tmpDate.angle           = Number(document.getElementById("angle").value);
    tmpDate.nWayAngle       = Number(document.getElementById("nWayAngle").value);
    tmpDate.bulletAngleAddAccValue  = Number(document.getElementById("angleAddAccValue").value);
    tmpDate.intrval                 = Number(document.getElementById("intrval").value);
    tmpDate.dips                    = Number(document.getElementById("dips").value);
	var target =
		String(tmpDate.index)					+ "\n" +
		String(tmpDate.bulletSpeed)			    + "\n" +
		String(tmpDate.bulletNum)				+ "\n" +
		String(tmpDate.nWayNum)				    + "\n" +
		String(tmpDate.angle)					+ "\n" +
		String(tmpDate.nWayAngle)				+ "\n" +
		String(tmpDate.bulletAngleAddAccValue)	+ "\n" +
		String(tmpDate.intrval)				    + "\n" +
		String(tmpDate.dips);

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

// テーブルにデータを追加.
document.getElementById("addTableButton").onclick = function () {

    // テキストの文字を配列にして取得.
    var table = document.getElementById("addText");
    var textValue = table.value;
    var list = textValue.split(/,|\s| |\t/);
    var listMax = list.length;

    // 無効な文字がないか確認.
    if (listMax < 9) return;
    for (i = 0; i < listMax; i++) if (isNaN(list[i]) == true) return;

    // テーブルに行を追加する.
    var table = document.getElementById("addTable");
    var newRow = table.insertRow();

    function addCell( s ) {
        var newCell = newRow.insertCell();
        var newText = document.createTextNode(s);
        newCell.appendChild(newText);
    }

    var i = 0;
    for (i = 0; i < listMax; i++) {
        addCell(list[i]);
    }


    var select = document.getElementById("tableNum");
    select.innerHTML = '';
    for (var i = 0; i < table.rows.length - 1; i++) {
        var option = document.createElement("option");
        option.text = String(i+1);
        option.value = i + 1;
        select.appendChild(option);
    }

}

// テーブルのデータを削除.
document.getElementById("numClearTableButton").onclick = function () {

    var select = document.getElementById("tableNum");

    var num = select.value;
    if (num == "") return;
    var table = document.getElementById('addTable');  //表のオブジェクトを取得
    var row_num = table.rows.length;    //表の行数を取得
    if (num < row_num) {
        //表に二行以上あるとき末尾行を削除（見出し行は削除しない）
        table.deleteRow(num);   //末尾行を削除
    }

    select.innerHTML = '';
    for (var i = 0; i < table.rows.length - 1; i++) {
        var option = document.createElement("option");
        option.text = String(i + 1);
        option.value = i + 1;
        select.appendChild(option);
    }
}

// テーブルのデータを全削除.
document.getElementById("clearTableButton").onclick = function () {

    var table = document.getElementById("addTable");
    var rowLen = table.rows.length;
    //上の行から削除していくと下の行がずれていくので下から検査
    for (var i = rowLen - 1; i > 0; i--) {
        table.deleteRow(i);
    }
    var select = document.getElementById("tableNum");
    select.innerHTML = '';
    for (var i = 0; i < table.rows.length - 1; i++) {
        var option = document.createElement("option");
        option.text = String(i + 1);
        option.value = i + 1;
        select.appendChild(option);
    }
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
	
	if (isPlay == true) {
	    var i = 0;
	    for (i = 0; i < weponNum; i++ ){
	        wepon[i].update(ctx);  
	    }
	}

	endCount++;
	if (endCount >= endFrame) isPlay = false;
}

var interval = setInterval( main, 10 );