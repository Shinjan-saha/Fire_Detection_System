import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:fl_chart/fl_chart.dart';
import 'secret.dart';

void main() => runApp(ThingSpeakDashboard());

class ThingSpeakDashboard extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
       debugShowCheckedModeBanner: false,
      title: 'Firealarm System Dashboard Dashboard',
      theme: ThemeData(
        brightness: Brightness.dark,
        primaryColor: Colors.blue,
        colorScheme: ColorScheme.dark().copyWith(
          primary: Colors.blue,
          secondary: Colors.blueAccent,
        ),
      ),
      home: DashboardScreen(),
    );
  }
}

class DashboardScreen extends StatefulWidget {
  @override
  _DashboardScreenState createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {

  final int results = 10;

  Future<List<dynamic>> fetchData(int fieldNumber) async {
    final url = Uri.parse(
        'https://api.thingspeak.com/channels/$channelId/fields/$fieldNumber.json?api_key=$readApiKey&results=$results');
    final response = await http.get(url);

    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      final feeds = data['feeds'] as List;
      final List<FlSpot> spots = [];
      final List<String> labels = [];

      for (var i = 0; i < feeds.length; i++) {
        final value = double.tryParse(feeds[i]['field$fieldNumber'] ?? '0') ?? 0;
        spots.add(FlSpot(i.toDouble(), value));
        labels.add(feeds[i]['created_at'] ?? ''); // Added null check
      }

      return [spots, labels];
    } else {
      throw Exception('Failed to load data');
    }
  }

  Widget buildChart(List<FlSpot> spots, String title) {
    return AspectRatio(
      aspectRatio: 1.7,
      child: Card(
        elevation: 4,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(8),
        ),
        color: const Color(0xff2c4260),
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            mainAxisAlignment: MainAxisAlignment.start,
            mainAxisSize: MainAxisSize.max,
            children: <Widget>[
              Text(
                title,
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 18,
                  fontWeight: FontWeight.bold,
                ),
              ),
              const SizedBox(height: 4),
              Expanded(
                child: LineChart(
                  LineChartData(
                    gridData: FlGridData(show: false),
                    titlesData: FlTitlesData(show: false),
                    borderData: FlBorderData(
                      show: true,
                      border: Border.all(color: Color.fromARGB(255, 4, 181, 54), width: 1),
                    ),
                    minX: 0,
                    maxX: spots.length.toDouble() - 1,
                    minY: 0,
                    maxY: spots.isNotEmpty
                        ? spots.map((spot) => spot.y).reduce((a, b) => a > b ? a : b) + 10
                        : 0,
                    lineBarsData: [
                      LineChartBarData(
                        spots: spots,
                        isCurved: true,
                        color: Color.fromARGB(255, 191, 254, 3),
                        barWidth: 2,
                        isStrokeCapRound: true,
                        belowBarData: BarAreaData(
                          show: true,
                          color: Color.fromARGB(255, 213, 246, 3).withOpacity(0.3),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Fire Alarm Dashboard'),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Column(
            children: [
              FutureBuilder(
                future: fetchData(1),
                builder: (context, snapshot) {
                  if (snapshot.connectionState == ConnectionState.waiting) {
                    return CircularProgressIndicator();
                  } else if (snapshot.hasError) {
                    return Text('Error: ${snapshot.error}');
                  } else if (!snapshot.hasData || (snapshot.data as List).isEmpty) {
                    return Text('No data available');
                  } else {
                    final spots = snapshot.data?[0] as List<FlSpot>;
                    return buildChart(spots, 'Temperature');
                  }
                },
              ),
              FutureBuilder(
                future: fetchData(2),
                builder: (context, snapshot) {
                  if (snapshot.connectionState == ConnectionState.waiting) {
                    return CircularProgressIndicator();
                  } else if (snapshot.hasError) {
                    return Text('Error: ${snapshot.error}');
                  } else if (!snapshot.hasData || (snapshot.data as List).isEmpty) {
                    return Text('No data available');
                  } else {
                    final spots = snapshot.data?[0] as List<FlSpot>;
                    return buildChart(spots, 'Humidity');
                  }
                },
              ),
              FutureBuilder(
                future: fetchData(3),
                builder: (context, snapshot) {
                  if (snapshot.connectionState == ConnectionState.waiting) {
                    return CircularProgressIndicator();
                  } else if (snapshot.hasError) {
                    return Text('Error: ${snapshot.error}');
                  } else if (!snapshot.hasData || (snapshot.data as List).isEmpty) {
                    return Text('No data available');
                  } else {
                    final spots = snapshot.data?[0] as List<FlSpot>;
                    return buildChart(spots, 'Gas Concentration');
                  }
                },
              ),
              FutureBuilder(
                future: fetchData(4),
                builder: (context, snapshot) {
                  if (snapshot.connectionState == ConnectionState.waiting) {
                    return CircularProgressIndicator();
                  } else if (snapshot.hasError) {
                    return Text('Error: ${snapshot.error}');
                  } else if (!snapshot.hasData || (snapshot.data as List).isEmpty) {
                    return Text('No data available');
                  } else {
                    final spots = snapshot.data?[0] as List<FlSpot>;
                    return buildChart(spots, 'Flame Status');
                  }
                },
              ),
              FutureBuilder(
                future: fetchData(5),
                builder: (context, snapshot) {
                  if (snapshot.connectionState == ConnectionState.waiting) {
                    return CircularProgressIndicator();
                  } else if (snapshot.hasError) {
                    return Text('Error: ${snapshot.error}');
                  } else if (!snapshot.hasData || (snapshot.data as List).isEmpty) {
                    return Text('No data available');
                  } else {
                    final spots = snapshot.data?[0] as List<FlSpot>;
                    return buildChart(spots, 'Buzzer On Time');
                  }
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}
