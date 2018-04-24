// Usage:
// nodejs generate.js filename.cpp 128 256
// 128 - line to start with
// 256 - line to end at

var filename = process.argv[2];
var rowStart = process.argv[3];
var rowEnd   = process.argv[4];


var fs = require('fs');

var text = fs.readFileSync(filename, 'utf-8');
var rows = text.split('\n').slice(rowStart, rowEnd);


function prepareRow(row){
	return row
		.replace(/\!/g, ' НЕ ')
		.replace(/&&/g, ' И ')
		.replace(/>/g, ' &gt; ')
		.replace(/</g, ' &lt; ')
}

function generateConnector(id1,id2, text, gluepointFrom){
	return ''+
		'<draw:connector draw:style-name="gr2" draw:text-style-name="P2" draw:layer="layout" draw:start-shape="'+id1+
		'" draw:start-glue-point="'+(gluepointFrom||6)+'" draw:end-shape="'+id2+'" draw:end-glue-point="4">'+
			'<text:p>'+(text||'')+'</text:p>'+
		'</draw:connector>';
}

var currentPosition = 0;
var sizeA = 7, sizeB = 3, interval = 2, intervalH = 1; //cm
var tabshift = 1;
var currentId = 0;
var currentHeight = 0;
var generatedText = '';


generatedText +=
	'	<draw:custom-shape draw:style-name="gr1" draw:text-style-name="P2" xml:id="id0" draw:id="id0" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+(sizeB/2)+'cm" svg:x="0cm" svg:y="0cm">'+
	'		<text:p text:style-name="P1">Начало</text:p>'+
    '		<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 0 10800 10800 21600 21600 10800" draw:text-areas="1060 3180 20540 18420" draw:type="flowchart-terminator" draw:enhanced-path="M 3470 21600 X 0 10800 3470 0 L 18130 0 X 21600 10800 18130 21600 Z N"/>'+
    '	</draw:custom-shape>';

generatedText +=
	generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';


currentHeight += sizeB / 2 + interval;


for(var i = 0; i < rows.length; i++){
	var row = rows[i];
	var tabdepth = row.match(/^\t*/)[0].length - tabshift;
	console.log(tabdepth, row);
	row = row.trim();
	row = row.split(/\/\//)[0];
	if(!row){
		continue;
	}
	if(row.match(/^\/\//)){
		continue;
	}
	if(row.match(/^[\{\}]\s*$/)){
		continue;
	}
	if(row.match(/[csf]out/)){
		currentId++;
		generatedText +=
			'   <draw:custom-shape  xml:id="id'+currentId+'" draw:id="id'+currentId+'" draw:style-name="gr1" draw:text-style-name="P2" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm">'+
			'		<text:p text:style-name="P1">' +
			''+			'Вывод ' + row.replace(/;$/,'').split(/\s*<<\s*/g).slice(1).join(', ') +
			''+	   '</text:p>'+
			'		<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 12960 0 10800 21600 8600 21600 19400 10800 2160 10800" draw:text-areas="4230 0 17370 21600" draw:type="flowchart-data" draw:enhanced-path="M 4230 0 L 21600 0 17370 21600 0 21600 4230 0 Z N"/>' +
			'	</draw:custom-shape>\n\n';
		currentHeight += sizeB + interval;

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';
		continue;
	}
	if(row.match(/[csf]in/)){
		currentId++;
		generatedText +=
			'   <draw:custom-shape  xml:id="id'+currentId+'" draw:id="id'+currentId+'" draw:style-name="gr1" draw:text-style-name="P2" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm">'+
			'		<text:p text:style-name="P1">' +
			''+			'Ввод ' + row.replace(/;$/,'').split(/\s*>>\s*/g).slice(1).join(', ') +
			''+	   '</text:p>'+
			'		<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 12960 0 10800 21600 8600 21600 19400 10800 2160 10800" draw:text-areas="4230 0 17370 21600" draw:type="flowchart-data" draw:enhanced-path="M 4230 0 L 21600 0 17370 21600 0 21600 4230 0 Z N"/>' +
			'	</draw:custom-shape>\n\n';
		currentHeight += sizeB + interval;

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';
		continue;
	}
	if(row.match(/if\s*\(/)){
		currentId++;
		generatedText +=
			'	<draw:custom-shape  xml:id="id'+currentId+'" draw:id="id'+currentId+'" draw:style-name="gr1" draw:text-style-name="P2" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm">'+
			'		<text:p text:style-name="P1">'+prepareRow(row.replace(/.*if\s*\(/,'').replace(/\)\s*\{.*/,''))+' ?</text:p>'+
			'		<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 0 10800 10800 21600 21600 10800 10800 0 0 10800 10800 21600 21600 10800" draw:text-areas="5400 5400 16200 16200" draw:type="flowchart-decision" draw:enhanced-path="M 0 10800 L 10800 0 21600 10800 10800 21600 0 10800 Z N"/>'+
			'	</draw:custom-shape>';


		currentHeight += sizeB + interval;

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+1), 'да', 1)+'\n\n';

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+2), 'нет', 5)+'\n\n';

		continue;
	}

	if(row.match(/return/)){
		currentId++;
		generatedText +=
			'	<draw:custom-shape  xml:id="id'+currentId+'" draw:id="id'+currentId+'" draw:style-name="gr1" draw:text-style-name="P2" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+(sizeB/2)+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm">'+
			'		<text:p text:style-name="P1"> Возврат '+row.replace(/^return\s*/,'').replace(/;$/,'')+'</text:p>'+
			'		<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 0 10800 10800 21600 21600 10800" draw:text-areas="1060 3180 20540 18420" draw:type="flowchart-terminator" draw:enhanced-path="M 3470 21600 X 0 10800 3470 0 L 18130 0 X 21600 10800 18130 21600 Z N"/>'+
			'	</draw:custom-shape>';


		currentHeight += sizeB + interval;

		generatedText +=
			generateConnector('id'+(currentId-1), 'id'+(currentId+1), 'нет', 1)+'\n\n';

		continue;
	}

	if(row.match(/(for|while)\s*\(/)){
		var heading = prepareRow(row.replace(/.*(for|while)\s*\(/,'').replace(/\)\s*\{.*/,''));
		currentId++;
		generatedText +=
			'	<draw:polygon  xml:id="id'+currentId+'" draw:id="id'+currentId+
					'" draw:style-name="gr1" draw:text-style-name="P1" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm" svg:viewBox="0 0 6000 3400" draw:points="0,1400 990,0 5010,0 6000,1400 6000,3410 0,3410">'+
			'		<text:p text:style-name="P1">'+heading+'</text:p>'+
			'	</draw:polygon>';

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';
		currentHeight += sizeB + interval;


		currentId++;
		generatedText +=
			'	<draw:polygon  xml:id="id'+currentId+'" draw:id="id'+currentId+
					'" draw:style-name="gr1" draw:text-style-name="P1" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm" svg:viewBox="0 0 6000 3400" draw:points="0,0 6000,0 6000,2000 5010,3400 990,3400 0,2000">'+
			'		<text:p text:style-name="P1">'+heading+'</text:p>'+
			'	</draw:polygon>';

		generatedText +=
			generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';
		currentHeight += sizeB + interval;

		continue;
	}

	row = row.replace(/;$/,'');
	if(row.match(/\+\+$/)){
		row = 'Увеличить ' + row.replace(/\+\+$/, '') + ' на 1';
	} else if(row.match(/\-\-$/)){
		row = 'Уменьшить ' + row.replace(/\+\+$/, '') + ' на 1';
	} else if(row.match(/\-=/)){
		row = 'Уменьшить ' + row.split(/\-\=/g)[0] + ' на ' + row.split(/\-\=/g)[1];
	} else if(row.match(/\+=/)){
		row = 'Увеличить ' + row.split(/\+\=/g)[0] + ' на ' + row.split(/\+\=/g)[1];
	}


	currentId++;
	generatedText +=
		'	<draw:custom-shape draw:style-name="gr1" draw:text-style-name="P2" xml:id="id'+currentId+'" draw:id="id'+currentId+'" draw:layer="layout" svg:width="'+sizeA+'cm" svg:height="'+sizeB+'cm" svg:x="'+(tabdepth*(intervalH+sizeA))+'cm" svg:y="'+currentHeight+'cm">'+
		'		<text:p text:style-name="P1">'+prepareRow(row)+'</text:p>'+
		'	<draw:enhanced-geometry svg:viewBox="0 0 21600 21600" draw:glue-points="10800 0 0 10800 10800 21600 21600 10800" draw:type="flowchart-process" draw:enhanced-path="M 0 0 L 21600 0 21600 21600 0 21600 0 0 Z N"/>'+
		'</draw:custom-shape>';
	generatedText +=
		generateConnector('id'+currentId, 'id'+(currentId+1))+'\n\n';
	currentHeight += sizeB + interval;


}


var template = fs.readFileSync(__dirname+'/block-wrapper.fodg', 'utf-8');
fs.writeFileSync(filename + '_' + rowStart + '--' + rowEnd + '.fodg', template.replace('<placeholder/>', generatedText));
