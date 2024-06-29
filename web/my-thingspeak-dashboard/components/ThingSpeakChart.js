// components/ThingSpeakChart.js
import { useEffect, useState } from 'react';
import axios from 'axios';
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS, CategoryScale, LinearScale, PointElement, LineElement, Title, Tooltip, Legend } from 'chart.js';

ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Title, Tooltip, Legend);

const ThingSpeakChart = ({ channelId, readApiKey, fieldNumber, chartTitle }) => {
  const [chartData, setChartData] = useState({});
  const [chartLabels, setChartLabels] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const url = `https://api.thingspeak.com/channels/${channelId}/fields/${fieldNumber}.json?api_key=${readApiKey}&results=20`;
        const response = await axios.get(url);
        const data = response.data.feeds;
        if (data) {
          const field = `field${fieldNumber}`;
          const labels = data.map(feed => new Date(feed.created_at).toLocaleString());
          const values = data.map(feed => parseFloat(feed[field]));
          setChartLabels(labels);
          setChartData({
            labels,
            datasets: [
              {
                label: chartTitle,
                data: values,
                fill: false,
                borderColor: 'rgb(75, 192, 192)',
                tension: 0.1,
              },
            ],
          });
        }
        setLoading(false);
      } catch (error) {
        setError(error);
        setLoading(false);
      }
    };

    fetchData();
  }, [channelId, readApiKey, fieldNumber, chartTitle]);

  if (loading) {
    return <p>Loading...</p>;
  }

  if (error) {
    return <p>Error loading data: {error.message}</p>;
  }

  return (
    <div>
      <Line data={chartData} />
    </div>
  );
};

export default ThingSpeakChart;
