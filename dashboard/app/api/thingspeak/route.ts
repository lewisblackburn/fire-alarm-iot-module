import { NextResponse } from "next/server";

const THINGSPEAK_BASE_URL = "https://api.thingspeak.com";

interface ThingSpeakRequest {
  field3?: string;
  field4?: string;
}

export async function POST(request: Request) {
  const writeApiKey = process.env.THINGSPEAK_WRITE_API_KEY;

  if (!writeApiKey) {
    return NextResponse.json(
      { error: "ThingSpeak Write API Key is not configured." },
      { status: 500 }
    );
  }

  try {
    const body: ThingSpeakRequest = await request.json();
    const { field3, field4 } = body;

    const params = new URLSearchParams({
      api_key: writeApiKey,
      field3: field3 || "0",
      field4: field4 || "0",
      field5: "1"
    });

    const response = await fetch(`${THINGSPEAK_BASE_URL}/update.json`, {
      method: "POST",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      body: params.toString(),
    });

    if (!response.ok) {
      throw new Error("Failed to send data to ThingSpeak.");
    }

    const data = await response.json();

    return NextResponse.json({
      message: "Data sent successfully to ThingSpeak.",
      data,
    });
  } catch (error: any) {
    console.error(error);
    return NextResponse.json(
      { error: error.message || "An error occurred while sending data." },
      { status: 500 }
    );
  }
}

export async function GET() {
  const channelId = process.env.THINGSPEAK_CHANNEL_ID;
  const readApiKey = process.env.THINGSPEAK_READ_API_KEY;

  if (!channelId || !readApiKey) {
    return NextResponse.json(
      { error: "ThingSpeak environment variables are not configured." },
      { status: 500 }
    );
  }

  try {
    const url = `${THINGSPEAK_BASE_URL}/channels/${channelId}/feeds.json?api_key=${readApiKey}&results=1`;

    const response = await fetch(url);

    if (!response.ok) {
      throw new Error("Failed to retrieve data from ThingSpeak.");
    }

    const data = await response.json();

    const latestFeed = data.feeds?.[0];
    if (!latestFeed) {
      return NextResponse.json(
        { error: "No data found in the ThingSpeak channel." },
        { status: 404 }
      );
    }

    const field3 = latestFeed.field3;
    const field4 = latestFeed.field4;
    const field5 = latestFeed.field5;

    return NextResponse.json({
      message: "Latest field values retrieved successfully.",
      data: {
        field3,
        field4,
        field5,
      },
    });
  } catch (error: any) {
    console.error(error);
    return NextResponse.json(
      { error: error.message || "An error occurred while retrieving data." },
      { status: 500 }
    );
  }
}

