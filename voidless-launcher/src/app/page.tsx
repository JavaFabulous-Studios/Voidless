import './globals.css'

export default function Home() {
  return (
    <div className="launcher-container">
      <aside className="sidebar">
        <div className="sidebar-icon">V</div>
        <div className="sidebar-icon">G</div>
        <div className="sidebar-icon">S</div>
        <div style={{ marginTop: 'auto', paddingBottom: '20px', fontSize: '0.8rem', opacity: 0.5 }}>v0.1.0</div>
      </aside>
      
      <main className="main-content">
        <section className="hero">
          <h1 className="title">VOIDLESS</h1>
          <p>The journey begins in the void.</p>
          <button className="play-button">PLAY</button>
        </section>

        <div className="news-grid">
          <div className="news-card">
            <h3>Engine Update v0.1</h3>
            <p>Initial cross-platform windowing system implemented.</p>
          </div>
          <div className="news-card">
            <h3>Release Date</h3>
            <p>Voidless is coming on May 25, 2026.</p>
          </div>
          <div className="news-card">
            <h3>Community</h3>
            <p>Join the Voidless community on Discord.</p>
          </div>
        </div>
      </main>
    </div>
  )
}
