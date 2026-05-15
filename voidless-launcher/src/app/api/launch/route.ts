import { NextResponse } from 'next/server';
import { exec } from 'child_process';
import path from 'path';

export async function POST() {
  try {
    // Navigate from voidless-launcher/src/app/api/launch to Voidless/build/Debug/Voidless.exe
    const gamePath = path.resolve(process.cwd(), '../Voidless/build/Debug/Voidless.exe');
    
    console.log('Launching game at:', gamePath);
    
    // Use start to launch without blocking the server
    exec(`start "" "${gamePath}"`, (error) => {
      if (error) {
        console.error('Failed to launch game:', error);
      }
    });

    return NextResponse.json({ success: true });
  } catch (error) {
    return NextResponse.json({ success: false, error: 'Failed to launch' }, { status: 500 });
  }
}
