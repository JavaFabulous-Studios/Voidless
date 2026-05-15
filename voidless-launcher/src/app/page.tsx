import './globals.css'

export default function Home() {
  return (
    <div className="launcher-container">
      <aside className="sidebar">
        <div className="sidebar-top">
          <div className="sidebar-icon active">V</div>
          <div className="sidebar-icon">G</div>
          <div className="sidebar-icon">S</div>
        </div>
        <div className="sidebar-bottom">
          <div className="sidebar-icon settings">⚙️</div>
          <div style={{ paddingBottom: '20px', fontSize: '0.8rem', opacity: 0.5 }}>v0.1.0</div>
        </div>
      </aside>
      
      <main className="main-content">
        <section className="hero">
          <h1 className="title">VOIDLESS</h1>
          <p>The journey begins in the void.</p>
          <button className="play-button">PLAY</button>
        </section>

        <div className="content-grid">
          <div className="news-section">
            <h2 className="section-title">Latest News</h2>
            <div className="news-grid">
              <div className="news-card">
                <div className="news-tag">UPDATE</div>
                <h3>Engine Update v0.1</h3>
                <p>Initial cross-platform windowing system implemented.</p>
              </div>
              <div className="news-card">
                <div className="news-tag">EVENT</div>
                <h3>Release Date</h3>
                <p>Voidless is coming on May 25, 2026.</p>
              </div>
              <div className="news-card">
                <div className="news-tag">COMMUNITY</div>
                <h3>Community</h3>
                <p>Join the Voidless community on Discord.</p>
                <a href="https://dsc.gg/voidless5" target="_blank" rel="noopener noreferrer" className="news-link">Join Now →</a>
              </div>
            </div>
          </div>

          <div className="side-section">
            <h2 className="section-title">Quick Links</h2>
            <div className="quick-links">
              <div className="link-card">Support</div>
              <div className="link-card">Documentation</div>
              <div className="link-card">Patch Notes</div>
            </div>
          </div>
        </div>
      </main>
    </div>
  )
}
