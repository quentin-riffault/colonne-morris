var x = 0;
var int = 3000;
$(".chkbox").click(function(){
	console.log(`${$(this).attr("id")} = ${$(this).prop("checked")}`);
	$.ajax({
		type:"GET",
        crossDomain:true,
		url:"http://192.168.0.143:4200",
		data:`${$(this).attr("id")}`,
		success: function(data){
			console.log(`Succes: ${JSON.stringify(data)}`)
		}
	})
})

$("#send_button").click(function(){
	$.ajax({
		url: "http://192.168.0.143:4200",
		timeout: 3000,
		type: "GET",
		data: `m=${$("#speed").val()}`,
		succes: function(){
			console.log(`Sent ${$("#speed").val}`)
		}
	})

})

function formatTime(ms){
	let m, j, h, mi, s;
	
	s = ms/1000;
	mi = s/60;
	h = mi/60;
	j = h/24;

	s %= 60;
	mi %= 60;
	h %=24;
	j %= 31;

	return(`${Math.floor(j)}j ${Math.floor(h)}h ${Math.floor(mi)}m ${Math.floor(s)}s`) 	

}

setInterval(function getJSON() {
    $.ajax({
        url: "http://192.168.0.143:4200", //Requête d'actualisation au serveur
        timeout: 3000,
        type:"GET",
        data:"s",
        success: function(data) {
            console.log(data);

            $("#connect").html("autorenew");
            if (x != 2) {
                let _ = $("#connect_tooltip");
                _.attr("data-tooltip", "Connexion réussie");
                _.tooltip();
                x = 2;
            }

            data.state[0] ? $("#b").prop("checked", "checked") : $("#b").prop("checked", 0);
            data.state[1] ? $("#v").prop("checked", "checked") : $("#v").prop("checked", 0);
            data.state[2] ? $("#l").prop("checked", "checked") : $("#l").prop("checked", 0);

            $("#speed-disp").prop("style", `width: ${(data.speed/250)*100}%`);

            $(".data")[0].innerHTML = `${data.temp} °C`;
            $(".data")[1].innerHTML = `${data.co2} ppm`;
            $(".data")[2].innerHTML = `${data.pm1} ppm`;
            $(".data")[3].innerHTML = formatTime(data.time);
        },
        error: function(jqXHR, status, e) {

            console.error("\n\n", jqXHR, "\n\n", status, "\n\n", e);

            $("#connect").html("block");

            if (x != 1) {
                let _ = $("#connect_tooltip");
                _.attr("data-tooltip", "Échec de la connexion");
                _.tooltip();
                x = 1;
            }
        }
    })

}, int)