// pages/index.js
import Head from 'next/head';
import ThingSpeakChart from '../components/ThingSpeakChart';

export default function Home() {
  const channelId = process.env.NEXT_PUBLIC_THINGSPEAK_CHANNEL_ID;
  const readApiKey = process.env.NEXT_PUBLIC_THINGSPEAK_READ_API_KEY;

  return (
    <div>
      <Head>
        <title>ThingSpeak Dashboard</title>
        <meta name="description" content="ThingSpeak Data Dashboard" />
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <main className="container">
        <h1>ThingSpeak Dashboard</h1>
        <div className="grid">
          <div className="card">
            <ThingSpeakChart channelId={channelId} readApiKey={readApiKey} fieldNumber={1} chartTitle="Temperature" />
          </div>
          <div className="card">
            <ThingSpeakChart channelId={channelId} readApiKey={readApiKey} fieldNumber={2} chartTitle="Humidity" />
          </div>
          <div className="card">
            <ThingSpeakChart channelId={channelId} readApiKey={readApiKey} fieldNumber={3} chartTitle="Gas Concentration" />
          </div>
          <div className="card">
            <ThingSpeakChart channelId={channelId} readApiKey={readApiKey} fieldNumber={4} chartTitle="Flame Status" />
          </div>
          <div className="card">
            <ThingSpeakChart channelId={channelId} readApiKey={readApiKey} fieldNumber={5} chartTitle="Buzzer On Time" />
          </div>
        </div>
      </main>
    </div>
  );
}
