var fgHtml = "";
var bgHtml = "";
var color;
var mousedown = false;
var width, height;
var tool = 0;
var mouseOnGrid = false;
var drawing = false;
var darkness = 0;
var pixelType = 0x2588;
var spriteName = "newSprite";

var historyColors = [];

function setName() {
	spriteName = document.getElementById("sprname").value;
}

function createGrid() {
	width = document.getElementById("width").value;
	height = document.getElementById("height").value;
	
	var grid = document.getElementById("gridContainer");
	while(grid.firstChild) {
		grid.removeChild(grid.firstChild);
	}
	
	grid.addEventListener("mouseover", function() {
		mouseOnGrid = true;
	});
	grid.addEventListener("mouseleave", function() {
		mouseOnGrid = false;
	});
	
	for(var y = 0; y < height; y++) {
		for(var x = 0; x < width; x++) {
			var item = document.createElement("div");
			item.className = "grid-item";
			item.classList.add("a" + y);
			item.id = y * width + x;
			item.value = "0";
			item.name = "0x2588";
			item.addEventListener("mouseover", function() {
				event.preventDefault;
				if(mousedown)
					changeColor(this.id);
			});
			item.addEventListener("mousedown", function() {
				event.preventDefault;
				changeColor(this.id);
			});
			item.style.border = "1px solid red";
			
			var topHalf = document.createElement("div");
			topHalf.className = "top-half";
			topHalf.style.backgroundColor = "#ffffff00";
			var bottomHalf = document.createElement("div");
			bottomHalf.className = "bottom-half";
			bottomHalf.style.backgroundColor = "#ffffff00";
			
			item.appendChild(topHalf);
			item.appendChild(bottomHalf);
			
			grid.appendChild(item);
		}
	}
}

function getHTMLColor() {
	var fgBlue = document.getElementById("fg-blue").checked;
	var fgGreen = document.getElementById("fg-green").checked;
	var fgRed = document.getElementById("fg-red").checked;
	var fgInt = document.getElementById("fg-int").checked;
	var bgBlue = document.getElementById("bg-blue").checked;
	var bgGreen = document.getElementById("bg-green").checked;
	var bgRed = document.getElementById("bg-red").checked;
	var bgInt = document.getElementById("bg-int").checked;
	var trans = document.getElementById("trans").checked;
	
	color = 0x0000;
	
	fgHtml = "";
	bgHtml = "";
	
	var darkcol;
	if(darkness == 0) darkcol = "ff";
	if(darkness == 1) darkcol = "ee";
	if(darkness == 2) darkcol = "dd";
	if(darkness == 3) darkcol = "cc";
	if(darkness == 4) darkcol = "aa";
	if(darkness == 5) darkcol = "88";
	if(darkness == 6) darkcol = "77";
	if(darkness == 7) darkcol = "66";
	
	if(fgBlue) { 
		color |= 0x0001;
		fgHtml = darkcol;
	} else fgHtml = "00";
	if(fgGreen) { 
		color |= 0x0002;
		fgHtml = darkcol + fgHtml;
	} else fgHtml = "00" + fgHtml;
	if(fgRed) { 
		color |= 0x0004;
		fgHtml = darkcol + fgHtml;
	} else fgHtml = "00" + fgHtml;
	if(fgInt) { 
		color |= 0x0008;
		//fgHtml = fgHtml.replace(new RegExp(coldark.charAt(0), 'g'), collight.charAt(0));
	}
	
	if(bgBlue) { 
		color |= 0x0010;
		bgHtml = darkcol;
	} else bgHtml = "00";
	if(bgGreen) { 
		color |= 0x0020;
		bgHtml = darkcol + bgHtml;
	} else bgHtml = "00" + bgHtml;
	if(bgRed) { 
		color |= 0x0040;
		bgHtml = darkcol + bgHtml;
	} else bgHtml = "00" + bgHtml;
	if(bgInt) { 
		color |= 0x0080;
		//bgHtml = bgHtml.replace(new RegExp(coldark.charAt(0), 'g'), collight.charAt(0));
	}
	
	if(darkness == 0 || darkness == 1) {
		pixelType = "0x2588";
	} else if(darkness == 2 || darkness == 3) {
		pixelType = "0x2593";
	} else if(darkness == 4 || darkness == 5) {
		pixelType = "0x2592";
	} else if(darkness == 6 || darkness == 7) {
		pixelType = "0x2591";
	}
	
	color++;
	if(trans) {
		color = 0;
		fgHtml = "00000000";
		bgHtml = "00000000";
	}
	
	fgHtml = "#" + fgHtml;
	bgHtml = "#" + bgHtml;
	
	pt.style.backgroundColor = fgHtml;
	pb.style.backgroundColor = bgHtml;
	if(trans) {
		pt.style.border = "1px solid red";
		pb.style.border = "1px solid red";
	} else {
		pt.style.border = "1px solid green";
		pb.style.border = "1px solid green";
	}
	
	console.log(fgHtml, bgHtml);
}

function changeColor(el) {
	if(drawing == false) {
		drawing = true;
		recordHistory();
	}
	
	if(tool == 0) {
		var square = document.getElementById(el);
		square.childNodes[0].style.backgroundColor = fgHtml;
		square.childNodes[1].style.backgroundColor = bgHtml;
		if(fgHtml.substring(7, 9) == "00")
			square.style.border = "1px solid red";
		else
			square.style.border = "1px solid green";
		square.value = color;
		square.name = pixelType;
	} else {
		var square = document.getElementById(el);
		var parent = square.parentNode;
		var index = Array.prototype.indexOf.call(parent.children, square);
		floodFill(index % width, Math.floor(index / width), square.value, color, square.name, pixelType);
	}
}

function exportData() {
	var grid = document.getElementById("gridContainer");
	var squares = grid.childNodes;
	var output = '#ifndef ' + spriteName.toUpperCase() + 'H\n#define ' + spriteName.toUpperCase() + 'H\n\n#include "gameEngine.h"\n\nsprite ' + spriteName + ' = {\n\t';
	
	output += width + ", /* Sprite width */\n\t" + height + ", /* Sprite height */\n\tnew wchar_t[" + (width*height) + "] { /* Sprite characters */\n\t\t";
	for(var i = 0; i < squares.length; i++) {
		output += squares[i].name + ", ";
		if((i+1) % width == 0) output += "\n\t\t";
	}
	output += "\n\t},\n\tnew WORD[" + (width*height) + "] { /* Sprite colors */\n\t\t";
	for(var i = 0; i < squares.length; i++) {
		output += squares[i].value + ", ";
		if((i+1) % width == 0) output += "\n\t\t";
	}
	output += "\n\t}\n};\n\n#endif";
	document.getElementById("exportbox").innerHTML = output;
	console.log(output);
}

function importData(data) {
	var grid = document.getElementById("gridContainer");
	var squares = grid.childNodes;
	
	colors = data.split(',');
	
	document.getElementById("width").value = colors[0];
	document.getElementById("height").value = colors[1];
	createGrid();
	
	for(var i = 2; i < colors.length; i++) {
		var hex = (parseInt(colors[i])-1).toString(2);
		
		var inp = hex;
		for(var j = 0; j < 8 - hex.length; j++) {
			inp = "0" + inp;
		}
		
		hex = inp;
		
		fgHtml = "";
		bgHtml = "";
		
		if(hex.substring(7, 8) == 1) { 
			fgHtml = "aa";
		} else fgHtml = "00";
		if(hex.substring(6, 7) == 1) { 
			fgHtml = "aa" + fgHtml;
		} else fgHtml = "00" + fgHtml;
		if(hex.substring(5, 6) == 1) { 
			fgHtml = "aa" + fgHtml;
		} else fgHtml = "00" + fgHtml;
		if(hex.substring(4, 5) == 1) { 
			fgHtml = fgHtml.replace(/a/g, "f");
		}
		
		if(hex.substring(3, 4) == 1) { 
			bgHtml = "aa";
		} else bgHtml = "00";
		if(hex.substring(2, 3) == 1) { 
			bgHtml = "aa" + bgHtml;
		} else bgHtml = "00" + bgHtml;
		if(hex.substring(1, 2) == 1) { 
			bgHtml = "aa" + bgHtml;
		} else bgHtml = "00" + bgHtml;
		if(hex.substring(0, 1) == 1) { 
			bgHtml = bgHtml.replace(/a/g, "f");
		}
		
		fgHtml = "#" + fgHtml;
		bgHtml = "#" + bgHtml;
		
		if(hex.includes("-")) {
			fgHtml = "#00000000";
			bgHtml = "#00000000";
			squares[i-2].style.border = "1px solid red";
			squares[i-2].value = 0;
		} else {
			squares[i-2].style.border = "1px solid green";
			squares[i-2].value = parseInt(colors[i]);
		}
		
		squares[i-2].childNodes[0].style.backgroundColor = fgHtml;
		squares[i-2].childNodes[1].style.backgroundColor = bgHtml;
	}
}

function floodFill(x, y, targetColor, replacementColor, targetDark, replDark) {
	var node = getSquare(x, y);
	if(node != null) {
		if(targetColor == replacementColor && targetDark == replDark) return;
		else if(node.value != targetColor) return;
		else if(node.name != targetDark) return;
		else {
			node.name = pixelType;
			node.value = replacementColor;
			node.childNodes[0].style.backgroundColor = fgHtml;
			node.childNodes[1].style.backgroundColor = bgHtml;
			if(fgHtml.substring(7, 9) == "00")
				node.style.border = "1px solid red";
			else
				node.style.border = "1px solid green";
		}
		floodFill(x, y-1, targetColor, replacementColor, targetDark, replDark);
		floodFill(x, y+1, targetColor, replacementColor, targetDark, replDark);
		floodFill(x-1, y, targetColor, replacementColor, targetDark, replDark);
		floodFill(x+1, y, targetColor, replacementColor, targetDark, replDark);
	}
	return;
}

function generateNoise() {
	for(var y = 0; y < height; y++) {
		for(var x = 0; x < width; x++) {
			if(getRndInteger(0, 2) == 1) {
				var node = getSquare(x, y);
				changeColor(node.id);
			}
		}
	}
}

function getRndInteger(min, max) {
	return Math.floor(Math.random() * (max - min) ) + min;
}

function setTool(el) {
	tool = el.value;
}

function setDarkness(el) {
	darkness = el.value;
	
	if(darkness == 0) document.getElementById("fg-int").checked = true;
	if(darkness == 1) document.getElementById("fg-int").checked = false;
	if(darkness == 2) document.getElementById("fg-int").checked = true;
	if(darkness == 3) document.getElementById("fg-int").checked = false;
	if(darkness == 4) document.getElementById("fg-int").checked = true;
	if(darkness == 5) document.getElementById("fg-int").checked = false;
	if(darkness == 6) document.getElementById("fg-int").checked = true;
	if(darkness == 7) document.getElementById("fg-int").checked = false;
	
	getHTMLColor();
}

function getSquare(x, y) {
	if(x >= 0 && x < width && y >= 0 && y < height) {
		var grid = document.getElementById("gridContainer");
		var squares = grid.childNodes;
		
		return squares[y * width + x];
	}
	return null;
}

function recordHistory() {
	if(mouseOnGrid) {
		var grid = document.getElementById("gridContainer");
		var historyDiv = document.getElementById("history");
		var historyGrid = document.createElement("div");
		historyGrid.id = "history" + historyDiv.childNodes.length;
		
		var squares = grid.childNodes;
		for(var i = 0; i < squares.length; i++) {
			var gridClone = squares[i].cloneNode(true);
			gridClone.value = squares[i].value;
			historyGrid.appendChild(gridClone);
		}
		
		historyDiv.appendChild(historyGrid);
	}
}

function placeHistory() {
	var grid = document.getElementById("gridContainer");
	var historyDiv = document.getElementById("history");
	if(historyDiv.childNodes.length > 0) {
		var historyGrid = document.getElementById("history" + (historyDiv.childNodes.length-1));
		
		while(grid.firstChild) {
			grid.removeChild(grid.firstChild);
		}
		
		var squares = historyGrid.childNodes;
		for(var i = 0; i < squares.length; i++) {
			var gridClone = squares[i].cloneNode(true);
			gridClone.addEventListener("mouseover", function() {
				event.preventDefault;
				if(mousedown)
					changeColor(this.id);
			});
			gridClone.addEventListener("mousedown", function() {
				event.preventDefault;
				changeColor(this.id);
			});
			grid.appendChild(gridClone);
			grid.childNodes[i].value = squares[i].value;
		}
		
		historyDiv.removeChild(historyGrid);
	}
}

window.onload = function() {
	document.body.addEventListener("mousedown", function() { mousedown = true; });
	document.body.addEventListener("mouseup", function() { mousedown = false; drawing = false; });
	document.body.addEventListener("keydown", function(e) {
		if(e.key === 'z' && e.ctrlKey) {
			e.preventDefault();
			placeHistory();
		}
	});
	document.getElementById("gridContainer").addEventListener("mousedown", function() { event.preventDefault(); });
	createGrid();
	getHTMLColor();
};